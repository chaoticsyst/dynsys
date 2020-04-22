#include <GL/gl.h>
#include <stdexcept>
#include <QOpenGLFramebufferObject>

#include "Camera.h"
#include "VideoEncoder.h"
#include "Preferences.h"


namespace VideoEncoder {

const AVPixelFormat INPUT_PIX_FORMAT = AV_PIX_FMT_RGB32;
const AVPixelFormat OUTPUT_PIX_FORMAT = AV_PIX_FMT_YUV420P;

VideoEncoder::VideoEncoder() :
    working{false},
    built{false} {}

bool VideoEncoder::isWorking() const {
    return working;
}

void VideoEncoder::startEncoding(int videoWidth, int videoHeight, const char *filename) {
    if (working) {
        endEncoding();
    }
    working = true;

    width = videoWidth;
    height = videoHeight;

    formatContext        = nullptr;
    stream               = nullptr;
    yuvFrame             = nullptr;
    rgbFrame             = nullptr;
    codecContext         = nullptr;
    convertFramesContext = nullptr;

    AVOutputFormat *outputFormat = nullptr;
    AVCodec *codec               = nullptr;
    AVDictionary *formatOptions  = nullptr;

    av_register_all();

//    av_log_set_level(AV_LOG_QUIET);

    formatContext = avformat_alloc_context();
    if (formatContext == nullptr) {
        throw std::bad_alloc();
    }

    outputFormat = av_guess_format(nullptr, filename, nullptr);
    if (outputFormat == nullptr) {
        throw std::logic_error("Could not determine a format of the video.");
    }

    formatContext->oformat = outputFormat;
    if (av_dict_set(&formatOptions, "brand", "mp42", 0) < 0) {
        throw std::logic_error("Could not set the video options.");
    }

    codec = avcodec_find_encoder_by_name("libx264");
    if (codec == nullptr) {
        throw std::logic_error("Could not find the codec.");
    }

    stream = avformat_new_stream(formatContext, codec);
    if (stream == nullptr) {
        throw std::logic_error("Could not create a new video stream.");
    }
    stream->time_base = (AVRational){1, 35};

    codecContext = stream->codec;

    if (codec->sample_fmts == nullptr) {
        codecContext->sample_fmt = AV_SAMPLE_FMT_S16;
    } else {
        codecContext->sample_fmt = codec->sample_fmts[0];
    }
    codecContext->width = width;
    codecContext->height = height;
    codecContext->time_base = stream->time_base;
    codecContext->pix_fmt = OUTPUT_PIX_FORMAT;
    codecContext->gop_size = 32;
    codecContext->level = 31;

    //set up extra options
    av_opt_set(codecContext->priv_data, "crf", "1", 0);
    av_opt_set(codecContext->priv_data, "profile", "main", 0);
    av_opt_set(codecContext->priv_data, "preset", "medium", 0);
    av_opt_set(codecContext->priv_data, "b-pyramid", "0", 0);
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        throw std::logic_error("Could not initialize a codec context.");
    }

    av_dump_format(formatContext, 0, filename, 1);
    if (avio_open(&formatContext->pb, filename, AVIO_FLAG_WRITE) < 0) {
        throw std::logic_error("Could not create or initalize an AVIO context.");
    }

    if (avformat_write_header(formatContext, &formatOptions) < 0) {
        throw std::logic_error("Could not write the stream header in the output file.");
    }

    rgbFrame = av_frame_alloc();
    if (rgbFrame == nullptr) {
        throw std::bad_alloc();
    }
    rgbFrame->width = width;
    rgbFrame->height = height;
    rgbFrame->format = INPUT_PIX_FORMAT;
    if (av_frame_get_buffer(rgbFrame, 1) < 0) {
        throw std::bad_alloc();
    }

    yuvFrame = av_frame_alloc();
    if (yuvFrame == nullptr) {
        throw std::bad_alloc();
    }
    yuvFrame->width = width;
    yuvFrame->height = height;
    yuvFrame->format = OUTPUT_PIX_FORMAT;
    if (av_frame_get_buffer(yuvFrame, 1) < 0) {
        throw std::bad_alloc();
    }

    convertFramesContext = sws_getContext(width, height, INPUT_PIX_FORMAT, width, height, OUTPUT_PIX_FORMAT,
                                          SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

    frameNumber = 0;

    built = true;
}

void VideoEncoder::endEncoding() {
    if (working == false) {
        return;
    }
    allStates.clear();

    while (built && writeFrame(nullptr) == true);
    if (formatContext != nullptr) {
        if (built) {
            av_write_trailer(formatContext);
        }
        if (formatContext->pb != nullptr) {
            avio_close(formatContext->pb);
        }
        avformat_free_context(formatContext);
    }
    if (yuvFrame != nullptr) {
        av_frame_free(&yuvFrame);
    }
    if (rgbFrame != nullptr) {
        av_frame_free(&rgbFrame);
    }
    if (convertFramesContext != nullptr) {
        sws_freeContext(convertFramesContext);
    }

    working = false;
    built = false;
}

VideoEncoder::~VideoEncoder() {
    endEncoding();
}

bool VideoEncoder::writeFrame(AVFrame *frame) {
    AVPacket packet;
    av_init_packet(&packet);
    if (frame != nullptr) {
        frame->pts = frameNumber++;
    }
    if (avcodec_send_frame(codecContext, frame) < 0) {
        return false;
    }
    if (avcodec_receive_packet(codecContext, &packet) < 0) {
        return false;
    }
    av_packet_rescale_ts(&packet, codecContext->time_base, stream->time_base);
    packet.stream_index = stream->index;
    if (av_interleaved_write_frame(formatContext, &packet) < 0) {
        return false;
    }
    return true;
}

void VideoEncoder::writeFrame(const QImage &image) {
    if (working == false) {
        return;
    }
    const uint8_t *data[8] = {
        image.constBits(),
        rgbFrame->data[1],
        rgbFrame->data[2],
        rgbFrame->data[3],
        rgbFrame->data[4],
        rgbFrame->data[5],
        rgbFrame->data[6],
        rgbFrame->data[7]
    };
    sws_scale(convertFramesContext, data, rgbFrame->linesize, 0, image.height(),
                                    yuvFrame->data, yuvFrame->linesize);

    while(writeFrame(yuvFrame) == false);
}

void VideoEncoder::writeState(const FrameState &state) {
    if (working == false) {
        return;
    }
    allStates << state;
}

void VideoEncoder::endEncoding(std::function<void (const QMatrix4x4 &, size_t)> drawFunc) {
    if (working == false) {
        endEncoding();
    }
    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    glViewport(0, 0, width, height);

    QOpenGLFramebufferObject buffer(width, height);
    buffer.bind();

    Camera::Camera camera;
    camera.recalculatePerspective(width, height);

    for (const auto &[position, target, time] : allStates) {
        camera.setPosition(position);
        camera.setTarget(target);
        drawFunc(camera.getMatrix(), time);

        writeFrame(buffer.toImage());
    }

    buffer.release();
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
}

} //namespace VideoEncoder


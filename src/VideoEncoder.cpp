#include <stdexcept>
#include "VideoEncoder.h"

namespace VideoEncoder {

#if LIBAVUTIL_VERSION_INT <= AV_VERSION_INT(52, 9, 0)
#define av_frame_alloc avcodec_alloc_frame
#endif

const AVPixelFormat INPUT_PIX_FORMAT = AV_PIX_FMT_RGB32;
const AVPixelFormat OUTPUT_PIX_FORMAT = AV_PIX_FMT_YUV420P;

VideoEncoder::VideoEncoder() :
    working{false} {}

bool VideoEncoder::isWorking() const {
    return working;
}

void VideoEncoder::startEncoding(int width, int height, const char *filename) {
    if (working) {
        endEncoding();
    }
    working = true;
    AVOutputFormat *outputFormat = nullptr;
    AVCodec *codec = nullptr;
    AVDictionary *formatOptions = nullptr;

    av_register_all();

    formatContext = avformat_alloc_context();
    if (formatContext == nullptr) {
        throw std::bad_alloc();
    }

    outputFormat = av_guess_format(nullptr, filename, nullptr);
    if (outputFormat == nullptr) {
        throw std::logic_error("Could not determine a format of the video.");
    }

    formatContext->oformat = outputFormat;
    if (av_dict_set(&formatOptions, "movflags", "faststart", 0) < 0) {
        throw std::logic_error("Could not set the video options.");
    }
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
    stream->time_base = (AVRational){1, 30};

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
    av_opt_set(codecContext->priv_data, "crf", "35", 0);
    av_opt_set(codecContext->priv_data, "profile", "main", 0);
    av_opt_set(codecContext->priv_data, "preset", "ultrafast", 0);
    av_opt_set(codecContext->priv_data, "b-pyramid", "0", 0);

    av_log_set_level(AV_LOG_QUIET);

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
}

void VideoEncoder::endEncoding() {
    if (working == false) {
        return;
    }
    while (write(nullptr) == true);

    av_write_trailer(formatContext);
    avio_close(formatContext->pb);
    avformat_free_context(formatContext);
//    avcodec_free_context(&codecContext);
    av_freep(&yuvFrame->data[0]);
//    av_frame_free(&yuvFrame);
//    av_freep(&rgbFrame->data[0]);
//    av_frame_free(&rgbFrame);
    sws_freeContext(convertFramesContext);

    working = false;
}

VideoEncoder::~VideoEncoder() {
    endEncoding();
}

bool VideoEncoder::write(AVFrame *frame) {
    AVPacket packet = {0};
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

void VideoEncoder::write(const QImage &image) {
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

    while(write(yuvFrame) == false);
}

} //namespace VideoEncoder


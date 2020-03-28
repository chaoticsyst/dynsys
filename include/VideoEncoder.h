#pragma

#include <QImage>

extern "C"{
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/opt.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
}

namespace VideoEncoder {

class VideoEncoder final {
public:
    VideoEncoder();
    ~VideoEncoder();

    VideoEncoder(const VideoEncoder &other)            = delete;
    VideoEncoder(VideoEncoder &&other)                 = delete;
    VideoEncoder &operator=(const VideoEncoder &other) = delete;
    VideoEncoder &operator=(VideoEncoder &&other)      = delete;

    void write(const QImage &image);

    void startEncoding(int width, int height, const char *filename);
    void endEncoding();

    bool isWorking() const;

private:
    AVFormatContext *formatContext;
    AVStream *stream;
    AVFrame *yuvFrame;
    AVFrame *rgbFrame;
    AVCodecContext *codecContext;
    SwsContext *convertFramesContext;

    size_t frameNumber;

    bool working;

    bool write(AVFrame *frame);
};

} //namespace VideoEncoder

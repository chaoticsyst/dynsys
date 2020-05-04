#pragma

#include <functional>
#include <QImage>
#include <QVector3D>
#include <QVector>

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavutil/opt.h>
    #include <libavutil/frame.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
}

namespace VideoEncoder {

struct FrameState {
    QVector3D position;
    QVector3D target;
    size_t time;
};

class VideoEncoder final {
public:
    VideoEncoder();
    ~VideoEncoder();

    VideoEncoder(const VideoEncoder &other)            = delete;
    VideoEncoder(VideoEncoder &&other)                 = delete;
    VideoEncoder &operator=(const VideoEncoder &other) = delete;
    VideoEncoder &operator=(VideoEncoder &&other)      = delete;

    void startEncoding(int videoWidth, int videoHeight, const char *filename);
    void endEncoding(std::function<void (const QMatrix4x4 &projMatrix, size_t time)> drawFunc,
                     std::function<void (int)> callback);

    void endEncoding();

    void writeState(const FrameState &state);

    bool isWorking() const;

private:
    AVFormatContext *formatContext;
    AVStream *stream;
    AVFrame *yuvFrame;
    AVFrame *rgbFrame;
    AVCodecContext *codecContext;
    SwsContext *convertFramesContext;

    size_t frameNumber;
    int width;
    int height;

    bool working;
    bool built;

    QVector<FrameState> allStates;

    bool writeFrame(AVFrame *frame);
    void writeFrame(const QImage &image);
};

} //namespace VideoEncoder

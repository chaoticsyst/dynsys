#include <utility>
#include <vector>
#include <algorithm>
#include <QFileInfo>

#include "PointsViewQGLWidget.hpp"

PointsViewQGLWidget::PointsViewQGLWidget(QWidget *parent) :
    QGLWidget{QGLFormat(), parent},
    prefs{&Preferences::defaultPreferences} {}

void PointsViewQGLWidget::setPreferences(const Preferences::Preferences *prefs_) {
    prefs = prefs_;
    locusController.setPreferences(prefs);
    cameraController.setPreferences(prefs);
}

QSize PointsViewQGLWidget::minimumSizeHint() const {
    return { 640, 480 };
}

QSize PointsViewQGLWidget::sizeHint() const {
    return { 1080, 720 };
}

void PointsViewQGLWidget::addNewLocus(QVector<QVector3D> &&points) {
    locusController.addLocus(std::move(points));
}

void PointsViewQGLWidget::setCurrentTime(const int currentTime_) {
    currentTime = currentTime_;
}

bool PointsViewQGLWidget::startVideoRecording(const QString &filename) {
    try {
        videoEncoder.startEncoding(prefs->video.width, prefs->video.height, filename.toStdString().c_str());

        return true;
    } catch (const std::exception &e) {
        videoEncoder.endEncoding();

        return false;
    }
}

void PointsViewQGLWidget::endVideoRecording(std::function<void (int)> callback) {
    auto drawFunc = [&lc = locusController](const QMatrix4x4 &projMatrix, size_t time) {
        lc.draw(projMatrix, time);
    };
    videoEncoder.endEncoding(drawFunc, std::move(callback));
}

void PointsViewQGLWidget::clearAll() {
    locusController.clear();
}

void PointsViewQGLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);

    qglClearColor(QColor(Qt::black));

    locusController.initialize();
}

void PointsViewQGLWidget::resizeGL(int width, int height) {
    cameraController.recalculatePerspective(width, height);
}

void PointsViewQGLWidget::paintGL() {
    locusController.draw(cameraController.getMatrix(), currentTime);

    if (videoEncoder.isWorking()) {
        videoEncoder.writeState({cameraController.getPosition(),
                                 cameraController.getTarget(),
                                 currentTime});
    }
}

void PointsViewQGLWidget::mouseMoveEvent(QMouseEvent *event) {
    cameraController.applyMouseMoveEvent(event);
}

void PointsViewQGLWidget::mousePressEvent(QMouseEvent *event) {
    cameraController.applyMousePressEvent(event);
}

void PointsViewQGLWidget::keyPressEvent(QKeyEvent *event) {
    cameraController.applyKeyPressEvent(event);

    if (event->key() == Qt::Key_R) {
        static auto getFileName = [](size_t number) {
            return QString::fromStdString("screenshot_" + std::to_string(number) + ".png");
        };

        static size_t screenshotNumber = 0;
        while (QFileInfo(getFileName(screenshotNumber)).exists()) {
            screenshotNumber++;
        }

        grabFrameBuffer().save(getFileName(screenshotNumber), "PNG");
    }
}

void PointsViewQGLWidget::keyReleaseEvent(QKeyEvent *event) {
    cameraController.applyKeyReleaseEvent(event);
}

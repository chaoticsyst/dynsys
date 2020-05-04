#include <vector>
#include <cmath>
#include <algorithm>

#include "PointsViewQGLWidget.h"

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
    videoEncoder.endEncoding(drawFunc, callback);
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
}

void PointsViewQGLWidget::keyReleaseEvent(QKeyEvent *event) {
    cameraController.applyKeyReleaseEvent(event);
}

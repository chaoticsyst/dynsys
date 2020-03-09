#include <vector>
#include <cmath>
#include <algorithm>

#include "PointsViewQGLWidget.h"
#include "Preferences.h"


PointsViewQGLWidget::PointsViewQGLWidget(QWidget *parent) :
    QGLWidget{QGLFormat(), parent} {}

QSize PointsViewQGLWidget::minimumSizeHint() const {
    return Preferences::MIN_WINDOW_SIZE;
}

QSize PointsViewQGLWidget::sizeHint() const {
    return Preferences::INIT_WINDOW_SIZE;
}

QColor getNextColor(size_t index) {
    long double func = Preferences::COLOR_FUNCTION_DELTA * (index + 1);
    return QColor(std::abs(std::sin(func)) * 255,
                     std::abs(std::cos(func) * std::sin(func)) * 255,
                     std::abs(std::cos(func)) * 255);
}

void PointsViewQGLWidget::addNewLocus(QVector<QVector3D> &&points) {
    QColor color = getNextColor(locusController.size());
    locusController.addLocus(Locus::Locus(std::move(points), color));
}

void PointsViewQGLWidget::setCurrentTime(const int currentTime_) {
    currentTime = currentTime_;
}

void PointsViewQGLWidget::clearAll() {
    locusController.clear();
}

void PointsViewQGLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(Qt::black));

    shaderProgram.addShaderFromSourceCode(QGLShader::Vertex, Preferences::VERTEX_SHADER);
    shaderProgram.addShaderFromSourceCode(QGLShader::Fragment, Preferences::FRAGMENT_SHADER);
    shaderProgram.link();
}

void PointsViewQGLWidget::resizeGL(int width, int height) {
    cameraController.recalculatePerspective(width, height);
}

void PointsViewQGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.bind();
    shaderProgram.setUniformValue(Preferences::MATRIX_NAME, cameraController.getMatrix());
    locusController.draw(shaderProgram, currentTime);
    shaderProgram.release();
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

#include <vector>
#include <cmath>
#include <algorithm>
#include "visual_opengl_widget.h"


#include <iostream>

// Window size constants
constexpr QSize MIN_WINDOW_SIZE = QSize(640, 480);
constexpr QSize INIT_WINDOW_SIZE = QSize(1080, 720);

VisualOpenGLWidget::VisualOpenGLWidget(QWidget *parent) :
    QGLWidget{QGLFormat(), parent} {}

QSize VisualOpenGLWidget::minimumSizeHint() const {
    return MIN_WINDOW_SIZE;
}

QSize VisualOpenGLWidget::sizeHint() const {
    return INIT_WINDOW_SIZE;
}

void VisualOpenGLWidget::addNewLocus(const QVector<QVector3D> &points) {
    locusController.addLocus(Locus::Locus(points, QVector<QVector3D>(points.size(), QVector3D(1, 1, 1))));
}

void VisualOpenGLWidget::setCurrentTime(const int currentTime_) {
    currentTime = currentTime_;
}

void VisualOpenGLWidget::clearAll() {
    locusController.clear();
}


const char *vertexShader =  "attribute highp vec4 vertex;"
                            "attribute highp vec4 color;"
                            "uniform highp mat4 matrix;"
                            "varying highp vec4 varyingColor;"
                            "void main(void)"
                            "{"
                            "    varyingColor = color;"
                            "    gl_Position = matrix * vertex;"
                            "}";

const char *fragmentShader = "varying highp vec4 varyingColor;\n"
                             "void main(void)\n"
                             "{\n"
                             "   gl_FragColor = varyingColor;\n"
                             "}";

void VisualOpenGLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(Qt::black));

    shaderProgram.addShaderFromSourceCode(QGLShader::Vertex, vertexShader);
    shaderProgram.addShaderFromSourceCode(QGLShader::Fragment, fragmentShader);
    shaderProgram.link();
}

void VisualOpenGLWidget::resizeGL(int width, int height) {
    cameraController.recalculatePerspective(width, height);
}

void VisualOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.bind();
    shaderProgram.setUniformValue("matrix", cameraController.getMatrix());
    locusController.draw(shaderProgram, currentTime);
    shaderProgram.release();
}

void VisualOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    cameraController.applyMouseMoveEvent(event);
}

void VisualOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    cameraController.applyMousePressEvent(event);
}

void VisualOpenGLWidget::keyPressEvent(QKeyEvent *event) {
    cameraController.applyKeyPressEvent(event);
}

void VisualOpenGLWidget::keyReleaseEvent(QKeyEvent *event) {
    cameraController.applyKeyReleaseEvent(event);
}

#include <vector>
#include <cmath>
#include <algorithm>
#include "visual_opengl_widget.h"


#include <iostream>

// Window size constants
constexpr QSize MIN_WINDOW_SIZE = QSize(640, 480);
constexpr QSize INIT_WINDOW_SIZE = QSize(1080, 720);

// Size of cubes (points)
constexpr double R = 0.005;

VisualOpenGLWidget::VisualOpenGLWidget(QWidget *parent) :
    QGLWidget{QGLFormat(), parent} {}

QSize VisualOpenGLWidget::minimumSizeHint() const {
    return MIN_WINDOW_SIZE;
}

QSize VisualOpenGLWidget::sizeHint() const {
    return INIT_WINDOW_SIZE;
}

void VisualOpenGLWidget::pushBackToPaint(const QVector<QVector3D> &points) {
    vertices << points;
}

void VisualOpenGLWidget::setCurrentTime(const int currentTime_) {
    currentTime = currentTime_;
}

void VisualOpenGLWidget::clearPoints() {
    vertices.clear();
}


const char *vertexShader =  "attribute highp vec4 vertex;\n"
                            "uniform highp mat4 matrix;\n"
                            "void main(void)\n"
                            "{\n"
                            "   gl_Position = matrix * vertex;\n"
                            "}";

const char *fragmentShader = "uniform mediump vec4 color;\n"
                             "void main(void)\n"
                             "{\n"
                             "   gl_FragColor = color;\n"
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
    shaderProgram.setUniformValue("color", QColor(Qt::white));
    shaderProgram.setAttributeArray("vertex", vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glPointSize(2);
    glDrawArrays(GL_POINTS, 0, std::min(vertices.size(), currentTime)); // У меня тут постоянно вылетает warning про устаршевий метод TODO: Заменить на более новый
    shaderProgram.disableAttributeArray("vertex");
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

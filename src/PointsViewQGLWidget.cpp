#include <vector>
#include <cmath>
#include <algorithm>
#include "PointsViewQGLWidget.h"

// Window size constants
constexpr QSize MIN_WINDOW_SIZE = QSize(640, 480);
constexpr QSize INIT_WINDOW_SIZE = QSize(1080, 720);

PointsViewQGLWidget::PointsViewQGLWidget(QWidget *parent) :
    QGLWidget{QGLFormat(), parent} {}

QSize PointsViewQGLWidget::minimumSizeHint() const {
    return MIN_WINDOW_SIZE;
}

QSize PointsViewQGLWidget::sizeHint() const {
    return INIT_WINDOW_SIZE;
}

void PointsViewQGLWidget::addNewLocus(const QVector<QVector3D> &points) {
    locusController.addLocus(Locus::Locus(points, QVector<QVector3D>(points.size(), QVector3D(1, 1, 1))));
}

void PointsViewQGLWidget::setCurrentTime(const int currentTime_) {
    currentTime = currentTime_;
}

void PointsViewQGLWidget::clearAll() {
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

void PointsViewQGLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(Qt::black));

    shaderProgram.addShaderFromSourceCode(QGLShader::Vertex, vertexShader);
    shaderProgram.addShaderFromSourceCode(QGLShader::Fragment, fragmentShader);
    shaderProgram.link();
}

void PointsViewQGLWidget::resizeGL(int width, int height) {
    cameraController.recalculatePerspective(width, height);
}

void PointsViewQGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.bind();
    shaderProgram.setUniformValue("matrix", cameraController.getMatrix());
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

#include <vector>
#include <cmath>
#include <algorithm>
#include "PointsViewQGLWidget.h"

// Window size constants
constexpr QSize MIN_WINDOW_SIZE = QSize(640, 480);
constexpr QSize INIT_WINDOW_SIZE = QSize(1080, 720);

constexpr long double COLOR_FUNCTION_DELTA = 0.01;

PointsViewQGLWidget::PointsViewQGLWidget(QWidget *parent) :
    QGLWidget{QGLFormat(), parent} {}

QSize PointsViewQGLWidget::minimumSizeHint() const {
    return MIN_WINDOW_SIZE;
}

QSize PointsViewQGLWidget::sizeHint() const {
    return INIT_WINDOW_SIZE;
}

QVector3D getNextColor(size_t index) {
    long double func = COLOR_FUNCTION_DELTA * (index + 1);
    return QVector3D(std::abs(std::sin(func)),
                     std::abs(std::cos(func) * std::sin(func)),
                     std::abs(std::cos(func)));
}

void PointsViewQGLWidget::addNewLocus(const QVector<QVector3D> &points) {
    QVector<QVector3D> colors = QVector<QVector3D>(points.size(), getNextColor(locusController.size()));
    locusController.addLocus(Locus::Locus(points, colors));
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

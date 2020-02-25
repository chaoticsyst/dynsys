#include <vector>
#include <iostream>
#include "visual_opengl_widget.h"
#include "Model.h"

// Model constants
const Point START_POINT = {1, 1, 1};
const int COUNT_POINTS = 200'000;
const int STEPS_PER_COUNT = 10;
const double TAU = 0.001;

// Timer constants
const int TIMER_INTERVAL = 1;

// Window size constants
const int WINDOW_SIZE_H = 640;
const int WINDOW_SIZE_W = 480;

// Size of cubes (points)
const double R = 0.005;

// Normalize constans (TODO: adaptive normalize)
const int DIV_NORMALIZE = 8;

VisualOpenGLWidget::VisualOpenGLWidget(QWidget *parent) :
    QGLWidget{QGLFormat(), parent}, alpha{25}, beta{-25}, distance{5}, last{0} {

    currentPoints = generate_points(START_POINT, COUNT_POINTS, STEPS_PER_COUNT, TAU);

    timer = new QTimer(this);
    timer->setInterval(TIMER_INTERVAL);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start();
}

VisualOpenGLWidget::~VisualOpenGLWidget() {
    delete timer;
}

QSize VisualOpenGLWidget::minimumSizeHint() const {
    return QSize(WINDOW_SIZE_H, WINDOW_SIZE_W);
}

QSize VisualOpenGLWidget::sizeHint() const {
    return QSize(WINDOW_SIZE_H, WINDOW_SIZE_W);
}

//TODO: rewrite this function
void addCube(QVector<QVector3D> &vertices, const QVector3D &center) {

    //cubes
    vertices << QVector3D(center.x() - R, center.y() - R, center.z() + R)
             << QVector3D(center.x() + R, center.y() - R, center.z() + R)
             << QVector3D(center.x() + R, center.y() + R, center.z() + R) // Front
             << QVector3D(center.x() + R, center.y() + R, center.z() + R)
             << QVector3D(center.x() - R, center.y() + R, center.z() + R)
             << QVector3D(center.x() - R, center.y() - R, center.z() + R)
             << QVector3D(center.x() + R, center.y() - R, center.z() - R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R)
             << QVector3D(center.x() - R, center.y() + R, center.z() - R) // Back
             << QVector3D(center.x() - R, center.y() + R, center.z() - R)
             << QVector3D(center.x() + R, center.y() + R, center.z() - R)
             << QVector3D(center.x() + R, center.y() - R, center.z() - R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R)
             << QVector3D(center.x() - R, center.y() - R, center.z() + R)
             << QVector3D(center.x() - R, center.y() + R, center.z() + R) // Left
             << QVector3D(center.x() - R, center.y() + R, center.z() + R)
             << QVector3D(center.x() - R, center.y() + R, center.z() - R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R)
             << QVector3D(center.x() + R, center.y() - R, center.z() + R)
             << QVector3D(center.x() + R, center.y() - R, center.z() - R)
             << QVector3D(center.x() + R, center.y() + R, center.z() - R) // Right
             << QVector3D(center.x() + R, center.y() + R, center.z() - R)
             << QVector3D(center.x() + R, center.y() + R, center.z() + R)
             << QVector3D(center.x() + R, center.y() - R, center.z() + R)
             << QVector3D(center.x() - R, center.y() + R, center.z() + R)
             << QVector3D(center.x() + R, center.y() + R, center.z() + R)
             << QVector3D(center.x() + R, center.y() + R, center.z() - R) // Top
             << QVector3D(center.x() + R, center.y() + R, center.z() - R)
             << QVector3D(center.x() - R, center.y() + R, center.z() - R)
             << QVector3D(center.x() - R, center.y() + R, center.z() + R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R)
             << QVector3D(center.x() + R, center.y() - R, center.z() - R)
             << QVector3D(center.x() + R, center.y() - R, center.z() + R) // Bottom
             << QVector3D(center.x() + R, center.y() - R, center.z() + R)
             << QVector3D(center.x() - R, center.y() - R, center.z() + R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R);

    //tetrahedrons
    /*vertices << QVector3D(center.x() + R, center.y() + R, center.z() + R) <<
                QVector3D(center.x() - R, center.y() - R, center.z() + R) <<
                QVector3D(center.x() + R, center.y() - R, center.z() - R) << //front
                QVector3D(center.x() + R, center.y() + R, center.z() + R) <<
                QVector3D(center.x() - R, center.y() + R, center.z() - R) <<
                QVector3D(center.x() - R, center.y() - R, center.z() + R) << //right
                QVector3D(center.x() + R, center.y() + R, center.z() + R) <<
                QVector3D(center.x() + R, center.y() - R, center.z() - R) <<
                QVector3D(center.x() - R, center.y() + R, center.z() - R) << //left
                QVector3D(center.x() - R, center.y() + R, center.z() - R) <<
                QVector3D(center.x() - R, center.y() - R, center.z() + R) <<
                QVector3D(center.x() + R, center.y() - R, center.z() - R);*/ //bottom
}

QVector3D pointFromStruct(const Point &point) {
    return QVector3D(point.x / DIV_NORMALIZE, point.y / DIV_NORMALIZE, point.z / DIV_NORMALIZE); //TODO: implement normalization
}

QVector<QVector3D> fromVector(const std::vector<Point> &points) {
    QVector<QVector3D> result;
    for (auto &point : points) {
        result.push_back(pointFromStruct(point));
    }
    return result;
}

void VisualOpenGLWidget::updateTime() {
    if (last == currentPoints.size()) {
        return;
    }
    for (size_t i = 0; i < STEPS_PER_COUNT; i++) {
        addCube(vertices, pointFromStruct(currentPoints[last++]));
    }
    repaint();
}

void VisualOpenGLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(Qt::black));

    shaderProgram.addShaderFromSourceCode(QGLShader::Vertex,
                                          "attribute highp vec4 vertex;\n"
                                          "uniform highp mat4 matrix;\n"
                                          "void main(void)\n"
                                          "{\n"
                                          "   gl_Position = matrix * vertex;\n"
                                          "}");
    shaderProgram.addShaderFromSourceCode(QGLShader::Fragment,
                                          "uniform mediump vec4 color;\n"
                                          "void main(void)\n"
                                          "{\n"
                                          "   gl_FragColor = color;\n"
                                          "}");
    shaderProgram.link();
}

void VisualOpenGLWidget::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }
    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.01, 1000); //Кирилл, разберись с константами, вообще непонятно
    glViewport(0, 0, width, height);
}

void VisualOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    shaderProgram.bind();

    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(alpha, 0, 1, 0); //Тут желательно тоже разобраться с константами
    cameraTransformation.rotate(beta, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0); // И тут везде тоже
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    shaderProgram.setUniformValue("matrix", pMatrix * vMatrix * mMatrix);
    shaderProgram.setUniformValue("color", QColor(Qt::white));
    shaderProgram.setAttributeArray("vertex", vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // У меня тут постоянно вылетает warning про устаршевий метод TODO: Заменить на более новый
    shaderProgram.disableAttributeArray("vertex");
    shaderProgram.release();
}

void VisualOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePosition = event->pos();
    event->accept();
}

void VisualOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360; // Это тоже вынести в константы
        }
        while (alpha >= 360) {
            alpha -= 360;
        }

        beta -= deltaY;
        if (beta < -90) {
            beta = -90;
        }
        if (beta > 90) {
            beta = 90;
        }

        updateGL();
    }
    lastMousePosition = event->pos();

    event->accept();
}

void VisualOpenGLWidget::wheelEvent(QWheelEvent *event) {
    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            distance *= 1.1; // Эти константы тоже вынести
        } else if (delta > 0) {
            distance *= 0.9;
        }

        updateGL();
    }

    event->accept();
}

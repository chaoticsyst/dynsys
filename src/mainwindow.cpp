#include <vector>
#include <iostream>
#include "mainwindow.h"
#include "Model.h"

MainWindow::MainWindow(QWidget *parent) :
    QGLWidget{QGLFormat(), parent}, alpha{25}, beta{-25}, distance{5} {}

QSize MainWindow::sizeHint() const {
    return QSize(640, 480);
}

//TO DO: rewrite this function
void addCube(QVector<QVector3D> &vertices, const QVector3D &center) {
    const double R = 0.005;

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
}

QVector<QVector3D> fromVector(const std::vector<Point> &points) {
    QVector<QVector3D> result;
    for (auto &point : points) {
        result.push_back(QVector3D(point.x, point.y, point.z));
    }
    return result;
}

void MainWindow::initializeGL() {
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

void MainWindow::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }
    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);
    glViewport(0, 0, width, height);
}

void MainWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    shaderProgram.bind();

    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    // TODO HERE
    for (auto &point : fromVector(currentPoints)) {
        addCube(vertices, point);
    }
    currentPoints.clear();
    //

    shaderProgram.setUniformValue("matrix", pMatrix * vMatrix * mMatrix);
    shaderProgram.setUniformValue("color", QColor(Qt::white));
    shaderProgram.setAttributeArray("vertex", vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    shaderProgram.disableAttributeArray("vertex");
    shaderProgram.release();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    lastMousePosition = event->pos();
    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360;
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

void MainWindow::wheelEvent(QWheelEvent *event) {
    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            distance *= 1.1;
        } else if (delta > 0) {
            distance *= 0.9;
        }

        updateGL();
    }

    event->accept();
}

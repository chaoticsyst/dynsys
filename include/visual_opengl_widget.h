#pragma once

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTime>
#include <QTimer>
#include "Camera.h"

class VisualOpenGLWidget : public QGLWidget {
public:
    explicit VisualOpenGLWidget(QWidget *parent = nullptr);
    ~VisualOpenGLWidget() = default;

    void clearPoints();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void setCurrentTime(const int currentTime_);

    void pushBackToPaint(const QVector<QVector3D> &points);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;


private:
    Q_OBJECT

    QGLShaderProgram shaderProgram;
    QVector<QVector3D> vertices;

    Camera::KeyboardAndMouseController cameraController;

    int currentTime;
};

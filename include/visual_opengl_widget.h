#pragma once

#include <vector>
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTime>
#include <QTimer>
#include "Model.h"
#include "Camera.h"

class VisualOpenGLWidget : public QGLWidget {
public:
    explicit VisualOpenGLWidget(QWidget *parent = nullptr);
    ~VisualOpenGLWidget() = default;

    void setPoints(const std::vector<Model::Point> &points);
    void appendPoints(const std::vector<Model::Point> &points);
    void clearPoints();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private slots:
    void updatePoints();
    void updateKeys();

private:
    Q_OBJECT

    QGLShaderProgram shaderProgram;
    QVector<QVector3D> vertices;

    Camera camera;

    QTimer *pointsTimer;
    QTimer *keysTimer;

    size_t lastPoint;
    QVector<QVector3D> pointsToPaint;

    QSet<qint32> keys;
};

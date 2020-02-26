#pragma once

#include <vector>
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTime>
#include <QTimer>
#include "Model.h"

class VisualOpenGLWidget : public QGLWidget {
public:
    explicit VisualOpenGLWidget(QWidget *parent = nullptr);
    ~VisualOpenGLWidget() = default;

    void setPoints(const std::vector<Point> &points);
    void appendPoints(const std::vector<Point> &points);
    void clearPoints();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private slots:
    void updateTime();

private:
    Q_OBJECT

    QMatrix4x4 pMatrix;
    QGLShaderProgram shaderProgram;
    QVector<QVector3D> vertices;

    double alpha;
    double beta;
    double distance;
    QPoint lastMousePosition;

    QTimer *timer;

    size_t lastPoint;
    QVector<QVector3D> pointsToPaint;
};

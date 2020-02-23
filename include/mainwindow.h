#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTime>
#include <QTimer>
#include "Model.h"

class MainWindow : public QGLWidget {
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSize sizeHint() const override;

    std::vector<Point> currentPoints;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

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

    size_t last;
};

#endif // MAINWINDOW_H

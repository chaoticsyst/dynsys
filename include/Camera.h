#pragma once

#include <QGLWidget>
#include <QVector3D>
#include <QMatrix4x4>

class Camera final {
public:
    Camera();
    ~Camera()                         = default;
    Camera(const Camera &)            = delete;
    Camera(Camera &&)                 = delete;
    Camera &operator=(const Camera &) = delete;
    Camera &operator=(Camera &&)      = delete;

    QMatrix4x4 getMatrix() const;

    void applyDeltaPosition(const QVector3D &delta);
    void setPosition(const QVector3D &position);
    void recalculatePerspective(int width, int height);
    void recalculateTarget(const QPoint &newMousePosition);

    void moveForward(float force);
    void moveRight(float force);
    void moveUp(float force);

    void resetMousePosition(const QPoint &point);

private:
    constexpr static QVector3D worldUp = QVector3D(0, 1, 0);

    QVector3D cameraPosition;
    QVector3D cameraTarget = QVector3D(0, 0, 0);

    QVector3D cameraForward;
    QVector3D cameraUp;
    QVector3D cameraRight;

    QMatrix4x4 perspectiveMatrix;

    float pitch;
    float yaw;
    QPoint lastMousePosition;

    void recalculateVectors();
    void normalizeAngles();
};

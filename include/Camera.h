#pragma once

#include <QGLWidget>
#include <QVector3D>
#include <QMatrix4x4>
#include <QSet>

namespace Camera {

class Camera final {
public:
    Camera();
    ~Camera() = default;

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

    void setDefault();

private:
    constexpr static QVector3D worldUp = QVector3D(0, 1, 0);

    QVector3D cameraPosition;
    QVector3D cameraTarget;

    QVector3D cameraForward;
    QVector3D cameraUp;
    QVector3D cameraRight;

    QMatrix4x4 perspectiveMatrix;

    float pitch;
    float yaw;
    QPoint lastMousePosition;

    bool invalidState;

    void recalculateVectors();
    void normalizeAngles();
};

class KeyboardAndMouseController final : public QObject {
public:
    KeyboardAndMouseController();
    ~KeyboardAndMouseController() = default;

    KeyboardAndMouseController(const KeyboardAndMouseController &)            = delete;
    KeyboardAndMouseController(KeyboardAndMouseController &&)                 = delete;
    KeyboardAndMouseController &operator=(const KeyboardAndMouseController &) = delete;
    KeyboardAndMouseController &operator=(KeyboardAndMouseController &&)      = delete;


    void applyKeyPressEvent(QKeyEvent *event);
    void applyKeyReleaseEvent(QKeyEvent *event);
    void applyMousePressEvent(QMouseEvent *event);
    void applyMouseMoveEvent(QMouseEvent *event);

    QMatrix4x4 getMatrix() const;
    void recalculatePerspective(int width, int height);

private slots:
    void updateKeys();

private:
    Q_OBJECT

    Camera camera;
    QSet<qint32> keys;
    QTimer *timer;
};

} //namespace Camera

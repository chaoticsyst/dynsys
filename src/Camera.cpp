#include <cmath>

#include "Camera.h"

namespace Camera {

constexpr QVector3D INIT_CAMERA_POSITION = { 0, 0, 5 };
constexpr QVector3D INIT_CAMERA_TARGET   = -INIT_CAMERA_POSITION;

constexpr float INIT_YAW   = -M_PI / 2;
constexpr float INIT_PITCH = 0;

constexpr float VERTICAL_ANGLE = 60;
constexpr float NEAR_PLANE     = 0.001;
constexpr float FAR_PLANE      = 1000;

constexpr float EPS       = 0.001;
constexpr float MAX_PITCH = M_PI / 2 - EPS;

Camera::Camera() :
    cameraPosition{INIT_CAMERA_POSITION},
    cameraTarget{INIT_CAMERA_TARGET},
    pitch{INIT_PITCH},
    yaw{INIT_YAW},
    invalidState{false} {

    recalculateVectors();
}

void Camera::recalculateVectors() {
    cameraForward = (-cameraTarget).normalized();
    cameraRight = QVector3D::crossProduct(worldUp, cameraForward).normalized();
    cameraUp = QVector3D::crossProduct(cameraForward, cameraRight).normalized();
}

void Camera::recalculatePerspective(int width, int height) {
    perspectiveMatrix.setToIdentity();
    perspectiveMatrix.perspective(VERTICAL_ANGLE, static_cast<float>(width) / height, NEAR_PLANE, FAR_PLANE);
    glViewport(0, 0, width, height);
}

void Camera::recalculateTarget(const QPoint &newMousePosition, float mouseSensitivity) {
    if (invalidState) {
        lastMousePosition = newMousePosition;
        invalidState = false;
        return;
    }
    float deltaX = (newMousePosition.x() - lastMousePosition.x()) * mouseSensitivity;
    float deltaY = (lastMousePosition.y() - newMousePosition.y()) * mouseSensitivity;
    yaw += deltaX;
    pitch += deltaY;
    normalizeAngles();

    cameraTarget.setX(std::cos(pitch) * std::cos(yaw));
    cameraTarget.setY(std::sin(pitch));
    cameraTarget.setZ(std::cos(pitch) * std::sin(yaw));
    cameraTarget.normalize();

    recalculateVectors();
    lastMousePosition = newMousePosition;
}

void Camera::normalizeAngles() {
    if (pitch > MAX_PITCH) {
        pitch = MAX_PITCH;
    }
    if (pitch < -MAX_PITCH) {
        pitch = -MAX_PITCH;
    }
}

QMatrix4x4 Camera::getMatrix() const {
    QMatrix4x4 matrix;
    matrix.lookAt(cameraPosition, cameraPosition + cameraTarget, worldUp);
    return perspectiveMatrix * matrix;
}

QVector3D Camera::getPosition() const {
    return cameraPosition;
}

QVector3D Camera::getTarget() const {
    return cameraTarget;
}

void Camera::applyDeltaPosition (const QVector3D &delta) {
    cameraPosition += delta;
    recalculateVectors();
}

void Camera::setPosition(const QVector3D &position) {
    cameraPosition = position;
    recalculateVectors();
}

void Camera::setTarget(const QVector3D &target) {
    cameraTarget = target;
    recalculateVectors();
}

void Camera::moveForward(float force) {
    cameraPosition += cameraForward * (-force);
    recalculateVectors();
}

void Camera::moveRight(float force) {
    cameraPosition += cameraRight * force;
    recalculateVectors();
}

void Camera::moveUp(float force) {
    cameraPosition += cameraUp * force;
    recalculateVectors();
}

void Camera::resetMousePosition(const QPoint &newMousePosition) {
    lastMousePosition = newMousePosition;
}

void Camera::setDefault() {
    cameraPosition = INIT_CAMERA_POSITION;
    cameraTarget   = INIT_CAMERA_TARGET;
    pitch          = INIT_PITCH;
    yaw            = INIT_YAW;
    invalidState = true;

    recalculateVectors();
}


KeyboardAndMouseController::KeyboardAndMouseController() :
    prefs{&Preferences::defaultPreferences} {

    timer = new QTimer(this);
    timer->setInterval(1);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateKeys()));
    timer->start();
}

void KeyboardAndMouseController::setPreferences(const Preferences::Preferences *prefs_) {
    prefs = prefs_;
}

QMatrix4x4 KeyboardAndMouseController::getMatrix() const {
    return camera.getMatrix();
}

QVector3D KeyboardAndMouseController::getPosition() const {
    return camera.getPosition();
}

QVector3D KeyboardAndMouseController::getTarget() const {
    return camera.getTarget();
}

void KeyboardAndMouseController::recalculatePerspective(int width, int height) {
    camera.recalculatePerspective(width, height);
}

void KeyboardAndMouseController::applyKeyPressEvent(QKeyEvent *event) {
    keys.insert(event->key());
    event->accept();
}

void KeyboardAndMouseController::applyKeyReleaseEvent(QKeyEvent *event) {
    keys.remove(event->key());
    event->accept();
}

void KeyboardAndMouseController::applyMousePressEvent(QMouseEvent *event) {
    camera.resetMousePosition(event->pos());
    event->accept();
}

void KeyboardAndMouseController::applyMouseMoveEvent(QMouseEvent *event) {
    camera.recalculateTarget(event->pos(), prefs->camera.sensitivity);
    event->accept();
}

void KeyboardAndMouseController::updateKeys() {
    float force = 1;
    if (keys.contains(Qt::Key_Shift)) {
        force *= 2;
    }
    if (keys.contains(Qt::Key_Control)) {
        force /= 2;
    }
    force *= prefs->camera.speed;

    if (keys.contains(Qt::Key_W)) {
        camera.moveForward(force);
    }
    if (keys.contains(Qt::Key_S)) {
        camera.moveForward(-force);
    }
    if (keys.contains(Qt::Key_D)) {
        camera.moveRight(force);
    }
    if (keys.contains(Qt::Key_A)) {
        camera.moveRight(-force);
    }
    if (keys.contains(Qt::Key_Q)) {
        camera.moveUp(force);
    }
    if (keys.contains(Qt::Key_E)) {
        camera.moveUp(-force);
    }
    if (keys.contains(Qt::Key_F)) {
        camera.setDefault();
    }
}

} //namespace Camera

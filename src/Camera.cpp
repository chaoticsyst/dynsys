#include <cmath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTime>
#include <QTimer>
#include "Camera.h"

namespace Camera {

constexpr float eps = 0.001;

constexpr float VERTICAL_ANGLE = 60.0;
constexpr float NEAR_PLANE = 0.001;
constexpr float FAR_PLANE = 1000;

constexpr float speedMove = 0.08;
constexpr float speedRotation = 0.01;

constexpr float MAX_PITCH = M_PI / 2 - eps;

constexpr int DELTA_TIME = 1;

Camera::Camera() :
    cameraPosition{0, 0, 5}, cameraTarget{-cameraPosition},  pitch{0}, yaw{-M_PI / 2} {

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

void Camera::recalculateTarget(const QPoint &newMousePosition) {
    float deltaX = (newMousePosition.x() - lastMousePosition.x()) * speedRotation;
    float deltaY = (lastMousePosition.y() - newMousePosition.y()) * speedRotation;

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

void Camera::applyDeltaPosition (const QVector3D &delta) {
    cameraPosition += delta;
    recalculateVectors();
}

void Camera::setPosition(const QVector3D &position) {
    cameraPosition = position;
    recalculateVectors();
}

void Camera::moveForward(float force) {
    cameraPosition += cameraForward * (-force) * speedMove;
    recalculateVectors();
}

void Camera::moveRight(float force) {
    cameraPosition += cameraRight * force * speedMove;
    recalculateVectors();
}

void Camera::moveUp(float force) {
    cameraPosition += cameraUp * force * speedMove;
    recalculateVectors();
}

void Camera::resetMousePosition(const QPoint &newMousePosition) {
    lastMousePosition = newMousePosition;
}


KeyboardAndMouseController::KeyboardAndMouseController() {
    timer = new QTimer(this);
    timer->setInterval(DELTA_TIME);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateKeys()));
    timer->start();
}

QMatrix4x4 KeyboardAndMouseController::getMatrix() const {
    return camera.getMatrix();
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
    camera.recalculateTarget(event->pos());
    event->accept();
}

void KeyboardAndMouseController::updateKeys() {
    if (keys.contains(Qt::Key_W)) {
        camera.moveForward(1);
    }
    if (keys.contains(Qt::Key_S)) {
        camera.moveForward(-1);
    }
    if (keys.contains(Qt::Key_D)) {
        camera.moveRight(1);
    }
    if (keys.contains(Qt::Key_A)) {
        camera.moveRight(-1);
    }
    if (keys.contains(Qt::Key_Q)) {
        camera.moveUp(1);
    }
    if (keys.contains(Qt::Key_E)) {
        camera.moveUp(-1);
    }
}

} //namespace Camera

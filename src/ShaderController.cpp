#include "ShaderController.h"

namespace ShaderController {

void ShaderController::initialize() {
    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, QString(":/VertexShader.vsh"));
    shaderProgram.addShaderFromSourceFile(QGLShader::Geometry, QString(":/GeometryShader.gsh"));
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, QString(":/FragmentShader.fsh"));
    shaderProgram.link();
}

void ShaderController::startWork() {
    shaderProgram.bind();
    shaderProgram.enableAttributeArray("vertex");
}

void ShaderController::endWork() {
    shaderProgram.disableAttributeArray("vertex");
    shaderProgram.release();
}

void ShaderController::setVertex() {
    shaderProgram.setAttributeArray("vertex", GL_FLOAT, 0, 3);
}

void ShaderController::setMatrix(const QMatrix4x4 &matrix) {
    shaderProgram.setUniformValue("matrix", matrix);
}

void ShaderController::setArcadeMode(bool enabled) {
    shaderProgram.setUniformValue("arcadeMode", enabled);
}

void ShaderController::setStartTailSize(float size) {
    shaderProgram.setUniformValue("startTailSize", size);
}

void ShaderController::setFinalTailSize(float size) {
    shaderProgram.setUniformValue("finalTailSize", size);
}

void ShaderController::setTailLength(size_t length) {
    shaderProgram.setUniformValue("tailLength", static_cast<int>(length));
}

void ShaderController::setStartVertexIndex(size_t index) {
    shaderProgram.setUniformValue("startIndex", static_cast<int>(index));
}

void ShaderController::setTailColoringMode(bool enabled) {
    shaderProgram.setUniformValue("tailColoringMode", enabled);
}

void ShaderController::setTrajectoryIndex(size_t index) {
    shaderProgram.setUniformValue("trajectoryIndex", static_cast<int>(index));
}

void ShaderController::setTrajectoriesNumber(size_t number) {
    shaderProgram.setUniformValue("trajectoriesNumber", static_cast<int>(number));
}

void ShaderController::setColors(const QVector<QVector4D> &colors) {
    shaderProgram.setUniformValue("colorsNumber", colors.size());
    shaderProgram.setUniformValueArray("colors", colors.constData(), colors.size());
}

void ShaderController::setInterpolationDistance(float distance) {
    shaderProgram.setUniformValue("interpolationDist", distance);
}

} //namespace ShaderController

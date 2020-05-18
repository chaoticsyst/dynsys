#include "ShaderController.hpp"

namespace ShaderController {

void ShaderController::initialize() {
    gshPoints = new QGLShader(QGLShader::Geometry, &shaderProgram);
    gshLines = new QGLShader(QGLShader::Geometry, &shaderProgram);

    gshPoints->compileSourceFile(QString(":/GeometryShaderPoints.gsh"));
    gshLines->compileSourceFile(QString(":/GeometryShaderLines.gsh"));

    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, QString(":/VertexShader.vsh"));
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, QString(":/FragmentShader.fsh"));
    setPrimitive(GL_LINE_STRIP);

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

void ShaderController::setPrimitive(GLenum primitive) {
    shaderProgram.release();

    bool isLines = shaderProgram.shaders().contains(gshLines);
    bool isPoints = shaderProgram.shaders().contains(gshPoints);
    if (primitive == GL_POINTS && !isPoints) {
        if (isLines) {
            shaderProgram.removeShader(gshLines);
        }
        shaderProgram.addShader(gshPoints);
    } else if (primitive == GL_LINE_STRIP && !isLines) {
        if (isPoints) {
            shaderProgram.removeShader(gshPoints);
        }
        shaderProgram.addShader(gshLines);
    }

    shaderProgram.bind();
}

} //namespace ShaderController

#pragma once

#include <QGLShaderProgram>

namespace ShaderController {

class ShaderController final {
public:
    ShaderController()  = default;
    ~ShaderController() = default;

    ShaderController(const ShaderController &)            = delete;
    ShaderController(ShaderController &&)                 = delete;
    ShaderController &operator=(const ShaderController &) = delete;
    ShaderController &operator=(ShaderController &&)      = delete;

    void initialize();

    void startWork();
    void endWork();

    void setMatrix(const QMatrix4x4 &matrix);
    void setVertex();
    void setArcadeMode(bool enabled);
    void setStartTailSize(float size);
    void setFinalTailSize(float size);
    void setTailLength(size_t length);
    void setStartVertexIndex(size_t index);
    void setColor(const QColor &color);
private:
    QGLShaderProgram shaderProgram;
};

} //class ShaderController

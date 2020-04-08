#version 300 es

in highp vec4 vertex;
uniform highp mat4 matrix;

uniform bool decreasingTailMode;
uniform float startTailSize;
uniform float finalTailSize;
uniform highp int length;
uniform highp int startIndex;

void main(void) {
    gl_Position = matrix * vertex;

    if (decreasingTailMode == true) {
        float delta = (finalTailSize - startTailSize) / float(length);
        gl_PointSize = startTailSize + delta * float(gl_VertexID - startIndex);
    }
}

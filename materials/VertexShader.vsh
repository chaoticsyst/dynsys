#version 300 es

in highp vec4 vertex;
uniform highp mat4 matrix;

uniform bool arcadeMode;
uniform highp float startTailSize;
uniform highp float finalTailSize;

uniform highp int tailLength;
uniform highp int startIndex;

flat out highp int vertexID;

void main(void) {
    gl_Position = matrix * vertex;

    if (arcadeMode == true) {
        float delta = (finalTailSize - startTailSize) / float(tailLength);
        gl_PointSize = startTailSize + delta * float(gl_VertexID - startIndex);
    }

    vertexID = gl_VertexID;
}

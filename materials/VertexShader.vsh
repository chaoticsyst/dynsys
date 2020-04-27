#version 330 core

in highp vec4 vertex;
uniform highp mat4 matrix;

uniform bool arcadeMode;
uniform highp float startTailSize;
uniform highp float finalTailSize;

uniform highp int tailLength;
uniform highp int startIndex;

//flat out highp int vertexID_ForFSH;

void main() {
    gl_Position = vertex;
    if (arcadeMode == true) {
        float delta = (finalTailSize - startTailSize) / float(tailLength);
        gl_PointSize = startTailSize + delta * float(gl_VertexID - startIndex);
    }

//    vertexID_ForFSH = gl_VertexID;
}

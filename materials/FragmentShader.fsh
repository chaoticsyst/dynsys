#version 330 core
#undef highp

uniform bool tailColoringMode;

uniform highp int tailLength;
uniform highp int startIndex;
uniform highp int trajectoryIndex;
uniform highp int trajectoriesNumber;

uniform highp int colorsNumber;
uniform highp vec4 colors[20];

flat in highp int vertexID_FSH;
flat in highp float vertexOffset;

out highp vec4 fragColor;

void main(void) {
    if (colorsNumber == 1) {
        fragColor = colors[0];
        return;
    }
    highp int index;
    highp int bunchSize;
    if (tailColoringMode == true) {
        index = vertexID_FSH - startIndex;
        bunchSize = (tailLength + colorsNumber - 2) / (colorsNumber - 1);
    } else {
        index = trajectoryIndex;
        bunchSize = (trajectoriesNumber + colorsNumber - 2) / (colorsNumber - 1);
    }
    highp int colorIndex = index / bunchSize;
    highp float colorPart = (float(index % bunchSize) + vertexOffset) / float(bunchSize);
    fragColor = colors[colorIndex] + colorPart * (colors[colorIndex + 1] - colors[colorIndex]);
}

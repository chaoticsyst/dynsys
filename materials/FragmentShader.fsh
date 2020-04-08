#version 300 es
#undef highp

uniform highp vec4 color;

out highp vec4 fragColor;

void main(void) {
    fragColor = color;
}

attribute highp vec4 vertex;
uniform highp mat4 matrix;

void main(void) {
    gl_Position = matrix * vertex;
}

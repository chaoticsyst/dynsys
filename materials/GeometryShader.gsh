#version 330 core

layout(lines_adjacency) in;
layout(points, max_vertices = 256) out;

uniform mat4 matrix;

void main() {
    float distance = distance(gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
    int cuts = int(min(256.0, max(3.0, 0.003 / distance)));
    mat4 lineMatrix = transpose(mat4(gl_in[0].gl_Position,
                                     gl_in[1].gl_Position,
                                     gl_in[2].gl_Position,
                                     gl_in[3].gl_Position));
    mat4 curveMatrix = transpose(mat4( 0.0,  2.0,  0.0,  0.0,
                                      -1.0,  0.0,  1.0,  0.0,
                                       2.0, -5.0,  4.0, -1.0,
                                      -1.0,  3.0, -3.0,  1.0));

    for (int i = 0; i <= cuts; i++) {
        float offset = float(i) / float (cuts);
        vec4 offsetVec = vec4(1.0, offset, offset * offset, offset * offset * offset);
        vec4 pos = 0.5 * offsetVec * curveMatrix * lineMatrix;
        gl_Position = matrix * pos;
        gl_PointSize = gl_in[1].gl_PointSize + float(offset) * (gl_in[2].gl_PointSize - gl_in[1].gl_PointSize);
        EmitVertex();
    }
}

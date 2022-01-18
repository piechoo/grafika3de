#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec4 a_vertex_color;
layout(location=0) out vec4 v_color;

void main() {
    gl_Position = a_vertex_position;
    v_color = a_vertex_color;
}

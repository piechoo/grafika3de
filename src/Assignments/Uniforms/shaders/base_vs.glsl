#version 450

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 a_vertex_color;

layout(std140, binding=1) uniform Transformations {
    vec2 scale;
    vec2 translation;
    mat2 rotation;
 };

out vec4 vertexColor;

void main() {
    vertexColor = a_vertex_color;
    gl_Position.xy =  rotation * (a_vertex_position.xy * scale) + translation;
    gl_Position.zw = a_vertex_position.zw;  
}

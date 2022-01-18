#version 410

layout(location=0) out vec4 vFragColor;
layout(location=0) in  vec4 v_color;

void main() {
    vFragColor = v_color;
}

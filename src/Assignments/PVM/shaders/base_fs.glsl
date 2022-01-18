#version 450

in vec4 vertexColor;

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color; 
};

layout(location = 0) out vec4 vFragColor;

void main() {
    vFragColor.a = vertexColor.a;
    vFragColor.rgb = strength*color*vertexColor.rgb; 
}
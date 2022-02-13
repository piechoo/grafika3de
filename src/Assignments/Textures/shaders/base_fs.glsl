#version 450

in vec4 vertexColor;

layout(std140, binding = 0) uniform Modifier {
    vec3  color; 
    float strength;
};

layout(location = 0) out vec4 vFragColor;

void main() {
    vFragColor.a = vertexColor.a;
    vFragColor.rgb = color*strength;
}
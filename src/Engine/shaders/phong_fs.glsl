#version 450

layout(location=0) out vec4 vFragColor;

#if __VERSION__ > 410
layout(std140, binding=0) uniform Modifiers {
#else
    layout(std140) uniform Color {
    #endif
    vec4  Kd;
    bool use_map_Kd;
};

in vec2 vertex_texcoords;
in vec3 vertex_normals_in_vs;
in vec3 vertex_coords_in_vs;

uniform sampler2D map_Kd;

const int MAX_POINT_LIGHTS=24;

struct PointLight {
    vec3 position_in_w_space;
    vec3 position_in_view_space;
    vec3 color;
    float intensity;
    float radius;
};  

layout(std140, binding=2) uniform Lights {
    vec3 ambient;
    uint n_p_lights;
    PointLight p_light[MAX_POINT_LIGHTS];
};

void main() {
    vec3 normal = normalize(vertex_normals_in_vs);
	vec3 light_vector = normalize(p_light[0].position_in_view_space - vertex_coords_in_vs);

    float diffuse = max(0.0, dot(normal, light_vector));
    
    vFragColor.a = Kd.a;
    vFragColor.rgb = diffuse * p_light[0].color * p_light[0].intensity * Kd.rgb;
}

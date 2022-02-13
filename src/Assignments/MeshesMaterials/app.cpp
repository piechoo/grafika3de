//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Engine/ColorMaterial.h"


void SimpleShapeApplication::init()
{
    xe::ColorMaterial::init();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    set_camera(new Camera);
    set_controler(new CameraControler(camera()));
    

    std::vector<GLfloat> vertices = {
        //podstawa 
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,  
        -0.5f, -0.5f, 0.5f, 

        0.5f, -0.5f, -0.5f,   
        -0.5f, -0.5f, 0.5f,    
        -0.5f, -0.5f, -0.5f,   

        //tylna sciana
        0.0f, 1.0f, 0.0f,  
        0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,
        //prawa sciana
        0.0f, 1.0f, 0.0f,   
        0.5f, -0.5f, 0.5f, 
        0.5f, -0.5f, -0.5f,  
        //lewa sciana
        0.0f, 1.0f, 0.0f,   
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, 0.5f,  
        //przednia sciana
        0.0f, 1.0f, 0.0f,   
        -0.5f, -0.5f, 0.5f,  
        0.5f, -0.5f, 0.5f, 
    };

    std::vector<GLushort> indices = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
    };

    mesh.add_submesh(0, 6, new xe::ColorMaterial(glm::vec4(0.78f,0.13f,0.13f, 1.0f)));
    mesh.add_submesh(6, 9, new xe::ColorMaterial(glm::vec4(0.011f, 0.239f, 0.827f, 1.0f)));
    mesh.add_submesh(9, 12, new xe::ColorMaterial(glm::vec4(0.862f, 0.635f, 0.925f, 1.0f)));
    mesh.add_submesh(12, 15, new xe::ColorMaterial(glm::vec4(0.101f, 0.917f, 0.109f, 1.0f)));
    mesh.add_submesh(15, 18, new xe::ColorMaterial(glm::vec4(1.0f, 0.984f, 0.619f, 1.0f)));
    
    mesh.allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    mesh.load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    mesh.allocate_index_buffer(indices.size()*sizeof(GLushort), GL_STATIC_DRAW);
    mesh.load_indices(0, indices.size()*sizeof(GLushort), indices.data());

    glGenBuffers(1, &pvm_buffer_handle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, pvm_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    float aspect = (float)w/h;
    float fov = glm::pi<float>()/4.0;
    float near = 0.1f;
    float far = 100.0f;
    camera()->set_aspect(aspect); 
    camera()->perspective(fov, aspect, near, far);
    camera()->look_at(glm::vec3{1.8f, -1.4f, 1.8f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0, 1.0, 0.0});

    mesh.vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);
}

void SimpleShapeApplication::frame(){
    auto PVM = camera()->projection() * camera()->view();
    glBindBuffer(GL_UNIFORM_BUFFER, pvm_buffer_handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    mesh.draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h); 
    camera()->set_aspect((float) w / h);
}

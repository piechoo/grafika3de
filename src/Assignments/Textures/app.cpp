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

#define STB_IMAGE_IMPLEMENTATION  1
#include "3rdParty/stb/stb_image.h"
#include "Engine/mesh_loader.h"
#include "Engine/ColorMaterial.h"

void SimpleShapeApplication::init()
{
    xe::ColorMaterial::init();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    set_camera(new Camera);
    set_controler(new CameraControler(camera()));
    

    // std::vector<GLfloat> vertices = {
    //     //tylna sciana
    //     0.0f, 1.0f, 0.0f,   0.0f,0.0f,//0
    //     0.5f, -0.5f, -0.5f, 0.1910f, 0.5f,//1
    //     -0.5f, -0.5f, -0.5f, 0.5f, 0.1910f,//2
    //     //prawa sciana
    //     0.0f, 1.0f, 0.0f,   0.0f, 0.1f,//3
    //     0.5f, -0.5f, 0.5f,  0.5f, 0.8090f,//4
    //     0.5f, -0.5f, -0.5f, 0.1910f, 0.5f,//5
    //     //lewa sciana
    //     0.0f, 1.0f, 0.0f,  1.0f,0.0f, //6
    //     -0.5f, -0.5f, -0.5f, 0.5f, 0.1910f,//7
    //     -0.5f, -0.5f, 0.5f, 0.8090f, 0.5f,//8
    //     //przednia sciana
    //     0.0f, 1.0f, 0.0f,  1.0f,1.0f, //9
    //     -0.5f, -0.5f, 0.5f, 0.8090f, 0.5f,//11
    //     0.5f, -0.5f, 0.5f,  0.5f, 0.8090f,//10
    //     //podstawa 
    //     0.5f, -0.5f, -0.5f, 0.1910f, 0.5f, //12
    //     0.5f, -0.5f, 0.5f,  0.5f, 0.8090f,//13
    //     -0.5f, -0.5f, 0.5f, 0.8090f, 0.5f,//14
    //     //podstawa
    //     0.5f, -0.5f, -0.5f, 0.1910f, 0.5f,//15
    //     -0.5f, -0.5f, 0.5f, 0.8090f, 0.5f,//16
    //     -0.5f, -0.5f, -0.5f, 0.5f, 0.1910f,//17
    // };

    // std::vector<GLushort> indices = {
    //     0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
    // };

    // // mesh.add_submesh(0, 3, new xe::ColorMaterial(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f)));
    // // mesh.add_submesh(3, 6, new xe::ColorMaterial(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
    // // mesh.add_submesh(6, 9, new xe::ColorMaterial(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
    // // mesh.add_submesh(9, 12, new xe::ColorMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    // mesh.add_submesh(12, 18, new xe::ColorMaterial(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)));
    // mesh.add_submesh(0, 3 );
    // mesh.add_submesh(3, 6);
    // mesh.add_submesh(6, 9 );
    // mesh.add_submesh(9, 12);
    // mesh.add_submesh(12, 18);
    
    // mesh.allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    // mesh.load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    // mesh.allocate_index_buffer(indices.size()*sizeof(GLushort), GL_STATIC_DRAW);
    // mesh.load_indices(0, indices.size()*sizeof(GLushort), indices.data());

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

    // mesh.vertex_attrib_pointer(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
    // mesh.vertex_attrib_pointer(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), 3* sizeof(GLfloat));
    //piramidka
        // mesh = *xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/pyramid.obj",
        //                                   std::string(ROOT_DIR) + "/Models");
    //swiat
    mesh = *xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/blue_marble.obj",
                                          std::string(ROOT_DIR) + "/Models");

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

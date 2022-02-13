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
#include "Engine/mesh_loader.h"
#include "Engine/ColorMaterial.h"
#include "Engine/PhongMaterial.h"

#define STB_IMAGE_IMPLEMENTATION  1
#include "3rdParty/stb/stb_image.h"

void SimpleShapeApplication::init()
{
    xe::ColorMaterial::init();
    xe::PhongMaterial::init();
    
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    set_camera(new Camera);
    set_controler(new CameraControler(camera()));

    auto pyramid = new xe::Mesh;
    
    pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj",
                                          std::string(ROOT_DIR) + "/Models");

    add_submesh(pyramid);

    // PointLight(position, color, intensity, radius)
    add_light(xe::PointLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 2.0f, 5.0f));
    add_ambient(glm::vec3(0.0f, 1.0f, 0.0f));
    num_of_lights++;

    glGenBuffers(1, &pvm_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, pvm_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::mat4x3), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, pvm_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &light_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);
	glBufferData(GL_UNIFORM_BUFFER,
                sizeof(glm::vec3) + sizeof(glm::uint) + sizeof(xe::PointLight) * num_of_lights,
                nullptr, GL_STATIC_DRAW
                ); 
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, light_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    float aspect = (float)w/h;
    float fov = glm::pi<float>()/4.0;
    float near = 0.1f;
    float far = 100.0f;
    camera()->set_aspect(aspect); 
    camera()->perspective(fov, aspect, near, far);
    camera()->look_at(glm::vec3{0.0f, 0.0f, 3.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0, 1.0, 0.0});

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);
}

void SimpleShapeApplication::frame(){
    auto M = glm::mat4(1.0f);
    auto PVM = camera()->projection() * camera()->view() * M;
    auto VM = camera()->view() * M;
    auto R = glm::mat3(VM);
    auto N = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1])); 

    glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ambient_), &ambient_);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(unsigned int), &num_of_lights);
    for(int i = 0; i < num_of_lights; i++){
        p_lights_[i].position_in_vs = VM * glm::vec4(p_lights_[i].position_in_ws, 1.0f);
    }
    glBufferSubData(GL_UNIFORM_BUFFER,
                    sizeof(ambient_) + sizeof(unsigned int),
                    sizeof(xe::PointLight) * num_of_lights,
                    p_lights_.data()
                    );
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, pvm_buffer_handle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &N[0]);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for(auto m: meshes_)
        m->draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h); 
    camera()->set_aspect((float) w / h);
}

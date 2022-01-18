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

void SimpleShapeApplication::init()
{
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
        {{GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program)
    {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
        //podstawa
        0.5f, 0.5f, 0.0f, 0.78f,0.13f,0.13f,
        -0.5f, 0.5f, 0.0f, 0.78f,0.13f,0.13f,
        -0.5f, -0.5f, 0.0f, 0.78f,0.13f,0.13f,

        -0.5f, -0.5f, 0.0f, 0.78f,0.13f,0.13f,
        0.5f, -0.5f, 0.0f, 0.78f,0.13f,0.13f,
        0.5f, 0.5f, 0.0f, 0.78f,0.13f,0.13f,

        //prawy
        0.5f, 0.5f, 0.0f, 0.011f, 0.239f, 0.827f,
        0.0f, 0.0f, -1.0f, 0.011f, 0.239f, 0.827f,
        0.5f, -0.5f, 0.0f, 0.011f, 0.239f, 0.827f,

        //dol
        0.5f, -0.5f, 0.0f, 1.0f, 0.984f, 0.619f,
        0.0f, 0.0f, -1.0f, 1.0f, 0.984f, 0.619f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.984f, 0.619f,

        //lewo
        -0.5f, -0.5f, 0.0f, 0.101f, 0.917f, 0.109f,
        0.0f, 0.0f, -1.0f, 0.101f, 0.917f, 0.109f,
        -0.5f, 0.5f, 0.0f, 0.101f, 0.917f, 0.109f,

        //gora
        -0.5f, 0.5f, 0.0f, 0.862f, 0.635f, 0.925f,
        0.0f, 0.0f, -1.0f, 0.862f, 0.635f, 0.925f,
        0.5f, 0.5f, 0.0f, 0.862f, 0.635f, 0.925f,


        // -0.5f, -0.5f, 0.0f, 0.25f,1.00f,0.10f,
        // -0.5f, 0.0f, 0.0f, 0.25f,1.00f,0.10f,
        // -0.5f, 0.0f, 0.0f, 0.25f,1.00f,0.10f,
        // 0.5f, 0.0f, 0.0f, 0.25f,1.00f,0.10f,
        // 0.5f, -0.5f, 0.0f, 0.25f,1.00f,0.10f,
        };
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::vector<GLushort> indices = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
    };
    
    GLuint i_buffer_handle;
    glGenBuffers(1, &i_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint color_trans_buffer_handle;
    glGenBuffers(1, &color_trans_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, color_trans_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); 
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, color_trans_buffer_handle); 
    float strength = 1.0;
    float color[3] = {1.0, 1.0, 1.0};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color);
    
    auto [w, h] = frame_buffer_size();
    GLuint pvm_buffer_handle;
    glGenBuffers(1, &pvm_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, pvm_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 Projection = glm::perspective(glm::half_pi<float>(), (float)w/h, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(glm::vec3{1.8f, -1.4f, 1.8f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0, 1.0, 0.0}); //widok z dolu pod skosem
     //glm::mat4 View = glm::lookAt(glm::vec3{0.0f, 2.0f, 0.1f}, glm::vec3{0.0f, -2.0f, 0.0f}, glm::vec3{0.0, 1.0, 0.0}); //widok z gory
     //glm::mat4 View = glm::lookAt(glm::vec3{0.0f, -2.0f, 0.1f}, glm::vec3{0.0f, 2.0f, 0.0f}, glm::vec3{0.0, 1.0, 0.0}); //widok z dolu
    glm::mat4 PVM = Projection * View * Model;
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, pvm_buffer_handle);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame()
{
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}

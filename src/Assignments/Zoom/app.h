//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"
#include "camera.h"

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

class SimpleShapeApplication : public xe::Application
{
public:
    Camera *camera_;
    
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera *camera) { camera_ = camera; }
    
    Camera *camera() { return camera_; }

    void scroll_callback(double xoffset, double yoffset) override {
         Application::scroll_callback(xoffset, yoffset);   
         camera()->zoom(yoffset / 30.0f);
     }

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
    }
}

private:
    GLuint vao_;
    GLuint u_pvm_buffer_;
};
#ifndef __GLFW_WINDOW_HANDLER_HPP_INCLUDED__
#define __GLFW_WINDOW_HANDLER_HPP_INCLUDED__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdbool>
#include <iostream>

#include "../types/camera.hpp"

extern const int WINDOW_WIDTH, WINDOW_HEIGHT;
extern float rotate_xz_angle;

void glfw_error_callback(int error, const char* description) {
  std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if(key == GLFW_KEY_A) {
      rotate_xz_angle -= 1.0f;
      if(rotate_xz_angle < -180.0f) {
        rotate_xz_angle += 360.0f;
      }
    } else if(key == GLFW_KEY_D) {
      rotate_xz_angle += 1.0f;
      if(rotate_xz_angle > 180.0f) {
        rotate_xz_angle -= 360.0f;
      }
    }
  }
}

GLFWwindow *initialize_window() {
  glfwSetErrorCallback(glfw_error_callback);

  if(!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(
    WINDOW_WIDTH, 
    WINDOW_HEIGHT, 
    "SPH Fluid Simulation", 
    nullptr, 
    nullptr
  );
  if(!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }

  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetKeyCallback(window, key_callback);

  return window;
}

void cleanup_window(GLFWwindow *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

#endif
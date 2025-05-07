#ifndef __GLFW_WINDOW_HANDLER_HPP_INCLUDED__
#define __GLFW_WINDOW_HANDLER_HPP_INCLUDED__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdbool>
#include <iostream>

#include "../camera.hpp"

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3

extern const int WINDOW_WIDTH, WINDOW_HEIGHT;
// extern bool edit_mode;
// extern camera_t camera;
// extern float lastX, lastY;
// extern bool firstMouse;

// void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
//   if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//     edit_mode = !edit_mode;

//     if(edit_mode) {
//       glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//     } else {
//       glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//     }
//   }
// }

// void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
//   if (firstMouse) {
//     lastX = xpos;
//     lastY = ypos;
//     firstMouse = false;
//   }

//   float xOffset = xpos - lastX;
//   float yOffset = lastY - ypos;

//   lastX = xpos;
//   lastY = ypos;

//   if(edit_mode) {
//     return ;
//   }

//   camera.process_mouse_movement(xOffset, yOffset);
// }

void glfw_error_callback(int error, const char* description) {
  std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
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

  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CCW);
  // glEnable(GL_DEPTH_TEST);
  // glDisable(GL_CULL_FACE);

  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glEnable(GL_BLEND);
  // glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // glfwSetCursorPosCallback(window, mouse_callback);
  // glfwSetKeyCallback(window, key_callback);

  return window;
}

void cleanup_window(GLFWwindow *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

#endif
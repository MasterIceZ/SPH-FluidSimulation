#ifndef __GLFW_WINDOW_HANDLER_HPP_INCLUDED__
#define __GLFW_WINDOW_HANDLER_HPP_INCLUDED__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>

extern const int WINDOW_WIDTH, WINDOW_HEIGHT;

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

  return window;
}

void cleanup_window(GLFWwindow *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

#endif
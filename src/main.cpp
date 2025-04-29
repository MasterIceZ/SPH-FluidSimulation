#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

#include "handler/glfw_window_handler.hpp"
#include "handler/imgui_handler.hpp"

#include "shader/shader.hpp"

#include "sample.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// static variables
float r_value = 0.0f, g_value = 0.0f, b_value = 0.0f;

signed main(int argc, char *argv[]) {
  GLFWwindow *window = initialize_window();
  initialize_imgui(window);

  GLuint shader_program = create_shader_program(
    "shader/vertex_shader.glsl", 
    "shader/fragment_shader.glsl"
  );
  
  GLuint VAO = sample_triangle_array();

  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    render_imgui();

    GLint colorLocation = glGetUniformLocation(shader_program, "inputColor");
    glUniform3f(colorLocation, r_value, g_value, b_value);

    render_imgui_draw_data();

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanup_imgui();
  cleanup_window(window);

  return 0;
}

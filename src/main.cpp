#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "handler/glfw_window_handler.hpp"
#include "handler/imgui_handler.hpp"
#include "handler/shader_handler.hpp"

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

    GLuint resolution_location = glGetUniformLocation(shader_program, "resolution");
    glUniform2f(resolution_location, (float) display_w, (float) display_h);

    float time = glfwGetTime();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, time, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));

    GLuint model_location = glGetUniformLocation(shader_program, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

    render_imgui();

    GLint colorLocation = glGetUniformLocation(shader_program, "inputColor");
    glUniform3f(colorLocation, r_value, g_value, b_value);

    render_imgui_draw_data();

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanup_imgui();
  cleanup_window(window);

  return 0;
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <cstdbool>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>

#include "handler/glfw_window_handler.hpp"
#include "handler/imgui_handler.hpp"
#include "handler/renderer.hpp"
#include "handler/shader_handler.hpp"

#include "types/particle.hpp"
#include "types/camera.hpp"

#include "utils.hpp"

#include "solver/solver.hpp"

// configuration
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float EPS = 0.0001f;
const glm::vec3 border_max = glm::vec3(0.5, 0.5, 0.5);
const glm::vec3 border_min = glm::vec3(-0.5, -0.5, -0.5);

// general settings
float rotate_xz_angle = -60.0f;
float smooth_length = 0.06f;
float time_step = 0.01f;
float gravity = 9.8f;
const float damp = 0.25f;

// particle properties
float mass = 0.02f;
float rest_density = 1.0f;
float gas_constant = 1.0f;
float viscosity = 1.04f;

glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) WINDOW_WIDTH/ (float) WINDOW_HEIGHT, 0.1f, 100.0f);
camera_t camera(
  glm::vec3(0.0f),  
  3.5f,
  -rotate_xz_angle,
  30.0f
);

signed main(int argc, char *argv[]) {
  GLFWwindow *window = initialize_window();
  initialize_imgui(window);

  std::vector<particle_t> particles;
  generate_random_particle(particles, -0.4f, 0.4f, 300);

  GLuint particle_shader = create_shader_program(
    "shader/particle/vertex_shader.glsl",
    "shader/particle/fragment_shader.glsl"
  );
  GLint loc_proj_particle = glGetUniformLocation(particle_shader, "projection");
  GLint loc_view_particle = glGetUniformLocation(particle_shader, "view");

  int bucket = 0;
  std::array<std::vector<particle_t>, 2> particles_buckets;

  particles_buckets[bucket] = particles;

  GLuint VAO, VBO;

  std::vector<glm::vec3> particles_positions = generate_particle_positions(particles_buckets[bucket]);
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
    GL_ARRAY_BUFFER, 
    particles_positions.size() * sizeof(glm::vec3),
    particles_positions.data(), 
    GL_DYNAMIC_DRAW
  );

  glVertexAttribPointer(
    0, 
    3, 
    GL_FLOAT, 
    GL_FALSE, 
    sizeof(glm::vec3), 
    (void *) 0
  );
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glPointSize(10.0f);

  GLuint border_shader = create_shader_program(
    "shader/border/vertex_shader.glsl",
    "shader/border/fragment_shader.glsl"
  );
  GLint loc_proj_border = glGetUniformLocation(border_shader, "projection");
  GLint loc_view_border = glGetUniformLocation(border_shader, "view");

  std::vector<glm::vec3> border_vertices = {
    border_min,
    glm::vec3(border_max.x, border_min.y, border_min.z),
    glm::vec3(border_max.x, border_min.y, border_max.z),
    glm::vec3(border_min.x, border_min.y, border_max.z),

    glm::vec3(border_min.x, border_max.y, border_min.z),
    glm::vec3(border_max.x, border_max.y, border_min.z),
    border_max,
    glm::vec3(border_min.x, border_max.y, border_max.z)
};

  std::vector<unsigned int> border_indices = {
    0, 1, 1, 2, 2, 3, 3, 0,
    4, 5, 5, 6, 6, 7, 7, 4,
    0, 4, 1, 5, 2, 6, 3, 7
  };

  GLuint border_VAO, border_VBO, border_EBO;

  glGenVertexArrays(1, &border_VAO);
  glGenBuffers(1, &border_VBO);
  glGenBuffers(1, &border_EBO);

  glBindVertexArray(border_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, border_VBO);
  glBufferData(
    GL_ARRAY_BUFFER, 
    border_vertices.size() * sizeof(glm::vec3),
    border_vertices.data(),
    GL_STATIC_DRAW
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    border_indices.size() * sizeof(unsigned int),
    border_indices.data(), 
    GL_STATIC_DRAW
  );

  glVertexAttribPointer(
    0, 
    3, 
    GL_FLOAT, 
    GL_FALSE, 
    sizeof(glm::vec3), 
    (void *) 0
  );
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    render_imgui();

    camera.yaw = rotate_xz_angle;
    camera.updateCameraPosition();
    glm::mat4 view = camera.get_view_matrix();

    particles_buckets[!bucket] = sph_solver(particles_buckets[bucket]);
    particles_positions = generate_particle_positions(particles_buckets[!bucket]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(
      GL_ARRAY_BUFFER, 
      0,
      particles_positions.size() * sizeof(glm::vec3),
      particles_positions.data()
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    render_imgui_draw_data();

    glUseProgram(particle_shader);
    glUniformMatrix4fv(loc_proj_particle, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(loc_view_particle, 1, GL_FALSE, glm::value_ptr(view));
    glBindVertexArray(VAO);
    glDrawArrays(
      GL_POINTS, 
      0, 
      static_cast<GLsizei> (particles_positions.size())
    );

    glUseProgram(border_shader);
    glUniformMatrix4fv(loc_proj_border, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(loc_view_border, 1, GL_FALSE, glm::value_ptr(view));
    glBindVertexArray(border_VAO);

    glDrawElements(
      GL_LINES, 
      static_cast<GLsizei> (border_indices.size()), 
      GL_UNSIGNED_INT, 
      0
    );

    glBindVertexArray(0);
    glUseProgram(border_shader);

    glfwSwapBuffers(window);
    glfwPollEvents();

    bucket = !bucket;
  }

  cleanup_imgui();
  cleanup_window(window);

  return 0;
}
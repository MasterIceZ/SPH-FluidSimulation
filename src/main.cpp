#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <array>
#include <iostream>
#include <cstdbool>

#include "particle.hpp"

#include "handler/glfw_window_handler.hpp"
#include "handler/imgui_handler.hpp"
#include "handler/shader_handler.hpp"
#include "handler/renderer.hpp"

#include "solver/solver.hpp"

#include "sample/sample_shader.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// static variables
float r_value = 0.0f, g_value = 0.0f, b_value = 0.0f;
const glm::vec3 border_max = glm::vec3(10.0f, 10.0f, 10.0f);
const glm::vec3 border_min = glm::vec3(0.0f, 0.0f, 0.0f);

// general settings
float smooth_length = 1.0f;
float time_step = 0.01f;
float gravity = 0.1f;
const float damp = 1.0f;

// particle properties
float mass = 1.0f;
float rest_density = 1000.0f;
float sound_speed = 1500.0f;
float particle_gamma = 7.0f;

signed main(int argc, char *argv[]) {
  GLFWwindow *window = initialize_window();
  initialize_imgui(window);

  std::vector<particle_t> particles;
  for (float x = 1.0f; x <= 3.0f; x += 0.5f) {
    for (float y = 1.0f; y <= 3.0f; y += 0.5f) {
      particles.push_back(
          {glm::vec3(x, y, 5), glm::vec3(0), glm::vec3(0), 0.0f, 0.0f});
    }
  }

  GLuint shader_program = create_shader_program(
    "shader/particle/vertex_shader.glsl", 
    "shader/particle/fragment_shader.glsl"
  );

  int bucket = 0;
  std::array<std::vector<particle_t>, 2> particles_buckets;

  particles_buckets[bucket] = particles;

  GLuint VAO, VBO;
  create_particle_buffers(VAO, VBO, 10000);

  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    GLuint resolution_location = glGetUniformLocation(shader_program, "resolution");
    glUniform2f(resolution_location, (float) display_w, (float) display_h);

    render_imgui();

    // Update particles
    particles_buckets[!bucket] = sph_solver(particles_buckets[bucket]);

    std::cerr << "BEFORE: " << particles_buckets[!bucket].size() << "\n";
    for(auto particle: particles_buckets[bucket]) {
      std::cerr << "Particle: " << particle.position.x << ", " << particle.position.y << ", " << particle.position.z << "\n";
    }
    std::cerr << "AFTER: " << particles_buckets[!bucket].size() << "\n";
    for(auto particle: particles_buckets[!bucket]) {
      std::cerr << "Particle: " << particle.position.x << ", " << particle.position.y << ", " << particle.position.z << "\n";
    }
    std::cerr << "=============\n" << "\n";

    GLint colorLocation = glGetUniformLocation(shader_program, "inputColor");
    glUniform3f(colorLocation, r_value, g_value, b_value);

    render_imgui_draw_data();

    // std::cerr << "Particles size: " << particles_buckets[!bucket].size() << std::endl;
    render_particles(particles_buckets[!bucket], VAO, VBO, shader_program);

    glfwSwapBuffers(window);
    glfwPollEvents();

    bucket = !bucket;
  }

  cleanup_imgui();
  cleanup_window(window);

  return 0;
}

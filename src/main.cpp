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
#include "particle.hpp"
#include "solver/solver.hpp"

// #include "sample/sample_shader.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float EPS = 0.0001f;

// static variables
float r_value = 0.0f, g_value = 0.0f, b_value = 0.0f;
const glm::vec3 border_max = glm::vec3(0.5, 0.5, 0.5);
const glm::vec3 border_min = glm::vec3(-0.5, -0.5, -0.5);

// general settings
float smooth_length = 0.045f;
float time_step = 0.01f;
float gravity = 9.8f;
const float damp = 0.3f;

// particle properties
float mass = 0.02f;
float rest_density = 1.0f;
float gas_constant = 1.0f;
float viscosity = 1.04f;

void show_particles(const std::vector<particle_t> &particles) {
  for (const auto &particle : particles) {
    std::cout << "Particle position: (" << particle.position.x << ", "
              << particle.position.y << ", " << particle.position.z << ")"
              << std::endl;
    std::cout << "Particle velocity: (" << particle.velocity.x << ", "
              << particle.velocity.y << ", " << particle.velocity.z << ")"
              << std::endl;
    std::cout << "Particle force: (" << particle.force.x << ", "
              << particle.force.y << ", " << particle.force.z << ")" << std::endl;
  }
  std::cout << "----------------------------------------" << std::endl;
}

std::vector<particle_t> normalize_particle(
    const std::vector<particle_t> &particles, float max_range) {
  std::vector<particle_t> normalized_particles;

  glm::vec3 range = glm::vec3(20, 20, 20);

  for (auto particle : particles) {
    particle.position.x =
        ((particle.position.x - border_min.x) / range.x) * 2.0f - 1.0f;
    particle.position.y =
        ((particle.position.y - border_min.y) / range.y) * 2.0f - 1.0f;
    particle.position.z =
        ((particle.position.z - border_min.z) / range.z) * 2.0f - 1.0f;

    normalized_particles.push_back(particle);
  }

  return normalized_particles;
}

void clamp_particle_positions(std::vector<particle_t> &particles) {
  for (auto &particle : particles) {
    particle.position.x =
        glm::clamp(particle.position.x, border_min.x, border_max.x);
    particle.position.y =
        glm::clamp(particle.position.y, border_min.y, border_max.y);
    particle.position.z =
        glm::clamp(particle.position.z, border_min.z, border_max.z);
  }
}

std::vector<glm::vec3> generate_particle_positions(const std::vector<particle_t> &particles) {
  std::vector<glm::vec3> positions;
  for (const auto &particle : particles) {
    positions.push_back(particle.position);
  }
  return positions;
}

signed main(int argc, char *argv[]) {
  GLFWwindow *window = initialize_window();
  initialize_imgui(window);

  // Generate particles
  std::vector<particle_t> particles;
  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> dist(-0.4f, 0.4f);

  // Generate 100 random points
  std::vector<glm::vec2> points;
  for (int i = 0; i < 500; ++i) {
    float x = dist(rng);
    float y = dist(rng);
    points.emplace_back(x, y);
  }
  for(int i = 0; i < points.size(); ++i) {
    particles.push_back({glm::vec3(points[i].x, points[i].y, 0), glm::vec3(0), glm::vec3(-1), 0.0f, 0.0f});
  }
  for (const auto &particle : particles) {
    std::cout << "Before Normalization: (" << particle.position.x << ", "
              << particle.position.y << ", " << particle.position.z << ")\n";
  }

  // particles = normalize_particle(particles, 15.0f);

  for (const auto &particle : particles) {
    std::cout << "After Normalization: (" << particle.position.x << ", "
              << particle.position.y << ", " << particle.position.z << ")\n";
  }

  GLuint shader_program = create_shader_program(
    "shader/particle/vertex_shader.glsl",
    "shader/particle/fragment_shader.glsl"
  );

  int bucket = 0;
  std::array<std::vector<particle_t>, 2> particles_buckets;

  particles_buckets[bucket] = particles;

  GLuint VAO, VBO;

  std::vector<glm::vec3> particles_positions = generate_particle_positions(particles_buckets[bucket]);
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, particles_positions.size() * sizeof(glm::vec3),
               particles_positions.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glPointSize(10.0f);

  GLuint border_shader = create_shader_program(
    "shader/vertex_shader.glsl",
    "shader/fragment_shader.glsl"
  );

  std::vector<glm::vec3> border_vertices = {
    // Bottom face
    border_min,
    glm::vec3(border_max.x, border_min.y, border_min.z),
    glm::vec3(border_max.x, border_min.y, border_max.z),
    glm::vec3(border_min.x, border_min.y, border_max.z),

    // Top face
    glm::vec3(border_min.x, border_max.y, border_min.z),
    glm::vec3(border_max.x, border_max.y, border_min.z),
    border_max,
    glm::vec3(border_min.x, border_max.y, border_max.z)
};

  std::vector<unsigned int> border_indices = {
    // Bottom face
    0, 1, 1, 2, 2, 3, 3, 0,

    // Top face
    4, 5, 5, 6, 6, 7, 7, 4,

    // Vertical edges
    0, 4, 1, 5, 2, 6, 3, 7
  };

  GLuint border_VAO, border_VBO, border_EBO;

  glGenVertexArrays(1, &border_VAO);
  glGenBuffers(1, &border_VBO);
  glGenBuffers(1, &border_EBO);

  glBindVertexArray(border_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, border_VBO);
  glBufferData(GL_ARRAY_BUFFER, border_vertices.size() * sizeof(glm::vec3),
              border_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
              border_indices.size() * sizeof(unsigned int),
              border_indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
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

    particles_buckets[!bucket] = sph_solver(particles_buckets[bucket]);
    // show_particles(particles_buckets[!bucket]);

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


    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particles_positions.size()));

    glUseProgram(border_shader);
    glBindVertexArray(border_VAO);

    glDrawElements(GL_LINES, static_cast<GLsizei>(border_indices.size()), GL_UNSIGNED_INT, 0);

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
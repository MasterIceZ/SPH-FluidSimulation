#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "../particle.hpp"

extern const glm::vec3 border_max;
extern const glm::vec3 border_min;

void render_particles(const std::vector<particle_t>& particles, GLuint VAO, GLuint VBO, GLuint shader_program) {
  std::vector<glm::vec3> positions;
  positions.reserve(particles.size());
  for(const auto& p : particles) {
    glm::vec3 normalized_position = (p.position / border_max) * 2.0f - 1.0f;
    positions.push_back(normalized_position);
  }

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * positions.size(), positions.data());

  glUseProgram(shader_program);
  glBindVertexArray(VAO);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glDrawArrays(GL_POINTS, 0, positions.size());
}

#endif
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

void update_particle_buffers(GLuint VBO, const std::vector<particle_t>& particles) {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER,    // target
                  0,                  // offset in bytes
                  sizeof(glm::vec3),  // just one point
                  particles.data());  // pointer to new data
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render_particles(const std::vector<particle_t>& particles, GLuint VAO, GLuint VBO, GLuint shader_program) {
  std::vector<glm::vec3> positions;
  positions.reserve(particles.size());
  for(const auto& p : particles) {
    glm::vec3 normalized_position = (p.position / border_max) * 2.0f - 1.0f;
    positions.push_back(normalized_position);
  }

  glUseProgram(shader_program);
  glBindVertexArray(VAO);

  glDrawArrays(GL_POINTS, 0, particles.size());

  glBindVertexArray(0);
  glUseProgram(0);
}

#endif
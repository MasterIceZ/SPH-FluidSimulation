#ifndef __SAMPLE_SHADER_HPP_INCLUDED__
#define __SAMPLE_SHADER_HPP_INCLUDED__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

void create_particle_buffers(GLuint& VAO, GLuint& VBO, size_t max_particles = 10000) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * max_particles, nullptr, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

#endif
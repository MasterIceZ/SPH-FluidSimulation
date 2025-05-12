#ifndef __OBJECTS_HPP__
#define __OBJECTS_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

extern const int WINDOW_WIDTH, WINDOW_HEIGHT;
extern const glm::vec3 border_max, border_min;

void add_particles(const std::vector<glm::vec3> &particles_positions, GLuint &VBO, GLuint &VAO) {
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
}

void add_border(const std::vector<glm::vec3> &border_vertices, const std::vector<unsigned int> &border_indices, GLuint &border_VAO, GLuint &border_VBO, GLuint &border_EBO) {
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
}

#endif
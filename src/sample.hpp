#ifndef __SAMPLE_HPP_INCLUDED__
#define __SAMPLE_HPP_INCLUDED__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLuint sample_triangle_array() {
  const float k = 1.0f / (2.0f * sqrtf(2.0f));

  float vertices[] = {
    // Positions
    k,   k,  k,   // Top vertex
    k,  -k, -k,  // Front-left vertex
   -k,  k, -k,  // Front-right vertex
   -k, -k,  k    // Back vertex
  };

  unsigned int indices[] = {
    0, 1, 2,  // Front face
    0, 2, 3,  // Right face
    0, 3, 1,  // Left face
    1, 3, 2   // Bottom face
  };

  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return VAO;
}

#endif
#ifndef __SAMPLE_HPP_INCLUDED__
#define __SAMPLE_HPP_INCLUDED__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLuint sample_triangle_array() {
  float vertices[] = {
    -0.5f, -0.5f, 0.0f, 
    0.5f, -0.5f, 0.0f, 
    0.0f, 0.5f, 0.0f
  };

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return VAO;
}

#endif
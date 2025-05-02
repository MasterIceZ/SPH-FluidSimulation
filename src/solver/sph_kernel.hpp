#ifndef __SPH_KERNEL_HPP_INCLUDED__
#define __SPH_KERNEL_HPP_INCLUDED__

#include <glm/glm.hpp>

#include <cmath>

inline float poly6_kernel(const glm::vec3 &r, float h) {
  float h2 = h * h;
  float r2 = glm::dot(r, r);
  if (r2 >= h2) return 0.0f;
  float coeff = 315.0f / (64.0f * M_PI * std::pow(h, 9));
  return coeff * std::pow(h2 - r2, 3);
}

inline glm::vec3 spikey_gradient(const glm::vec3 &r, float h) {
  float r_length = glm::length(r);
  if(r_length > 0 && r_length <= h) {
    float coeff = -45.0f / (M_PI * std::pow(h, 6));
    return coeff * std::pow(h - r_length, 2) * (r / r_length);
  }
  return glm::vec3(0.0f);
}

inline float viscosity_laplacian(const glm::vec3 &r, float h) {
  float r_length = glm::length(r);
  if(r_length > 0 && r_length <= h) {
    float coeff = 45.0f / (M_PI * std::pow(h, 6));
    return coeff * (h - r_length);
  }
  return 0.0f;
}

#endif
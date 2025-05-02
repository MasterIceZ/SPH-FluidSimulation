#ifndef __SPH_TYPES_HPP_INCLUDED__
#define __SPH_TYPES_HPP_INCLUDED__

#include <glm/glm.hpp>

#include <vector>

struct particle_t {
  glm::vec3 position, velocity, acceleration;
  float density, pressure;
};

struct solver_state_t {
  std::vector<particle_t> particles;
};

#endif
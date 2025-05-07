#ifndef __PARTICLE_HPP_INCLUDED__
#define __PARTICLE_HPP_INCLUDED__

#include <glm/glm.hpp>

struct particle_t {
  glm::vec3 position, velocity, force;
  float density, pressure;
};

#endif
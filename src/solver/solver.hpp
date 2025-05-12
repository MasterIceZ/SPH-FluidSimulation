#ifndef __SOLVER_HPP__
#define __SOLVER_HPP__

#include <vector>
#include <algorithm>

#include "../types/particle.hpp"
#include "../types/spatial_hashtable.hpp"

// TODO: USE SPATIAL HASHTABLE INSTEAD OF BRUTE FORCE

#include "smooth_kernel.hpp"

// general settings
extern float smooth_length;
extern float time_step;
extern float gravity;
extern const glm::vec3 border_min;
extern const glm::vec3 border_max;
extern const float damp;
extern const float EPS;

// particle properties
extern float mass;
extern float rest_density;
extern float gas_constant;
extern float particle_gamma;
extern float viscosity;

std::vector<particle_t> calculate_density_and_pressure(const std::vector<particle_t> &particles, const spatial_hash_table_t &spatial)
{
  std::vector<particle_t> new_particles = particles;
  for (int i = 0; i < (int)particles.size(); ++i)
  {
    particle_t &particle = new_particles[i];

    particle.density = mass * w_poly6(glm::vec3(0.0f));

    std::vector<int> neighbors = spatial.find_neighbors(particle.position);
    for (auto j : neighbors)
    {
      if (i == j)
      {
        continue;
      }
      const particle_t &other = particles[j];

      glm::vec3 r = other.position - particle.position;
      if (glm::length(r) > smooth_length)
      {
        continue;
      }

      particle.density += mass * w_poly6(r);
    }
    particle.pressure = gas_constant * (particle.density - rest_density);
    // std::cout << "Debug Density + Pressure" << std::endl;
    // std::cout << particle.density << " " << particle.pressure << std::endl;
  }
  return new_particles;
}

std::vector<particle_t> calculate_forces(const std::vector<particle_t> &particles, const spatial_hash_table_t &spatial)
{
  std::vector<particle_t> new_particles = particles;
  for (int i = 0; i < (int)particles.size(); ++i)
  {
    particle_t &particle = new_particles[i];

    particle.force = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 viscosity_force = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 pressure_force = glm::vec3(0.0f, 0.0f, 0.0f);

    std::vector<int> neighbors = spatial.find_neighbors(particle.position);
    for (auto j : neighbors)
    {
      if (i == j)
      {
        continue;
      }

      const particle_t &other = particles[j];

      glm::vec3 r = other.position - particle.position;
      float r1 = glm::length(r);
      if (r1 <= EPS || r1 > smooth_length)
      {
        continue;
      }

      glm::vec3 v = other.velocity - particle.velocity;
      viscosity_force += viscosity * mass * (v / other.density) * w_viscosity(r);

      float p = particle.pressure + other.pressure;
      glm::vec3 direction = glm::normalize(r);
      pressure_force += -direction * mass * (p / (2.0f * other.density)) * w_spiky(r);
 
      // if (r1 < 0.15f)
      // {
      //   glm::vec3 repulsion = glm::normalize(r) * (0.15f - r1) * 1000.0f;
      //   particle.force -= repulsion;
      // }
    }
    particle.force = pressure_force + viscosity_force;
    // particle.force = glm::clamp(particle.force, -100000.0f, 100000.0f);
  }
  return new_particles;
}

std::vector<particle_t> sph_solver(const std::vector<particle_t> &particles)
{
  std::vector<particle_t> new_particles;
  std::vector<particle_t> new_particles_tmp;

  spatial_hash_table_t spartial(smooth_length);
  for (int i = 0; i < (int)particles.size(); ++i)
  {
    const particle_t &particle = particles[i];
    spartial.insert(i, particle.position);
  }

  new_particles_tmp = calculate_density_and_pressure(particles, spartial);
  new_particles = calculate_forces(new_particles_tmp, spartial);

  for (auto &particle : new_particles)
  {
    glm::vec3 acceleration = particle.force / particle.density;
    // if (particle.density > 0.0f)
    // {
    //   acceleration = particle.force / particle.density;
    // }

    acceleration += glm::vec3(0.0f, -gravity, 0.0f);

    particle.velocity += acceleration * time_step;
    // particle.velocity = glm::clamp(particle.velocity, -10000.0f, 10000.0f);

    particle.position += particle.velocity * time_step;

    for (int axis = 0; axis < 3; ++axis)
    {
      if (particle.position[axis] < border_min[axis])
      {
        particle.position[axis] = border_min[axis] + 0.001f;
        if (particle.velocity[axis] < 0.0f)
        {
          particle.velocity[axis] *= -damp;
        }
      }
      else if (particle.position[axis] > border_max[axis])
      {
        particle.position[axis] = border_max[axis] - 0.001f;
        if (particle.velocity[axis] > 0.0f)
        {
          particle.velocity[axis] *= -damp;
        }
      }
    }
  }
  return new_particles;
}

#endif
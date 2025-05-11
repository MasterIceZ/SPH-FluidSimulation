#ifndef __SOLVER_HPP__
#define __SOLVER_HPP__

#include <vector>
#include <algorithm>

#include "../particle.hpp"

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

std::vector<particle_t> calculate_density_and_pressure(const std::vector<particle_t> &particles) {
  std::vector<particle_t> new_particles = particles;
  for(int i=0; i<(int) particles.size(); ++i) {
    particle_t &particle = new_particles[i];
    particle.density = mass * w_poly6(glm::vec3(0.0f));
    for(int j=0; j<(int) particles.size(); ++j) {
      if(i == j) {
        continue;
      }
      const particle_t &other = particles[j];
      glm::vec3 r = particle.position - other.position;
      if(glm::length(r) > smooth_length) {
        continue;
      }
      particle.density += mass * w_poly6(r);
    }
    particle.pressure = gas_constant * (particle.density - rest_density);
  }
  return new_particles;
}

std::vector<particle_t> calculate_forces(const std::vector<particle_t> &particles) {
  std::vector<particle_t> new_particles = particles;
  for(int i=0; i<(int) particles.size(); ++i) {
    // std::cerr << "OLD FORCE: " << new_particles[i].force.x << ", "
    //           << new_particles[i].force.y << ", " << new_particles[i].force.z
    //           << std::endl;
    particle_t &particle = new_particles[i];
    particle.force = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 viscosity_force = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 pressure_force = glm::vec3(0.0f, 0.0f, 0.0f);
    for(int j=0; j<(int) particles.size(); ++j) {
      if(i == j) {
        continue;
      }
      const particle_t &other = particles[j];
      glm::vec3 r = particle.position - other.position;
      // std::cerr << "D:"  << glm::length(r) << std::endl;
      if(glm::length(r) <= EPS || glm::length(r) > smooth_length) {
        continue;
      }
      std::cerr << "COLLIDE" << std::endl;
      glm::vec3 v = other.velocity - particle.velocity;
      viscosity_force += viscosity * mass * (v / other.density) * w_viscosity(r);
      float p = particle.pressure + other.pressure;
      glm::vec3 direction = glm::normalize(r);
      pressure_force -= direction * mass * (p / (2.0f * other.density)) * w_spiky(r);
    }
    particle.force = pressure_force + viscosity_force;
    // std::cerr << "PRESSURE FORCE: " << pressure_force.x << ", "
    //           << pressure_force.y << ", " << pressure_force.z << std::endl;
    // std::cerr << "VISCOSITY FORCE: " << viscosity_force.x << ", "
    //           << viscosity_force.y << ", " << viscosity_force.z << std::endl;
    // std::cerr << "NEW FORCE: " << new_particles[i].force.x << ", "
    //           << new_particles[i].force.y << ", " << new_particles[i].force.z
    //           << std::endl;
  }
  return new_particles;
}

std::vector<particle_t> sph_solver(const std::vector<particle_t> &particles) {
  std::vector<particle_t> new_particles;

  new_particles = calculate_density_and_pressure(particles);
  new_particles = calculate_forces(new_particles);

  for(auto &particle: new_particles) {
    glm::vec3 acceleration = particle.force / particle.density;

    if(particle.density > 0.0f) {
      acceleration = particle.force / particle.density;
    }

    acceleration += glm::vec3(0.0f, -gravity, 0.0f);

    particle.velocity += acceleration * time_step;
    particle.velocity = glm::clamp(particle.velocity, -10.0f, 10.0f);
    
    particle.position += particle.velocity * time_step;

    for (int axis = 0; axis < 3; ++axis) {
      if(particle.position[axis] < border_min[axis]) {
          particle.position[axis] = border_min[axis];
          particle.velocity[axis] *= -damp;
      }
      else if(particle.position[axis] > border_max[axis]) {
          particle.position[axis] = border_max[axis];
          particle.velocity[axis] *= -damp;
      }
    }

    if(glm::any(glm::isnan(particle.position))) {
      std::cerr << "Particle position is NaN!" << std::endl;
      for(int i=0; i<(int) particle.position.length(); ++i) {
        if(glm::isnan(particle.position[i])) {
          std::cerr << "Force: " << particle.force[i] << std::endl;
          std::cerr << "Velocity: " << particle.velocity[i] << std::endl;
          std::cerr << "Density: " << particle.density << std::endl;
          std::cerr << "Pressure: " << particle.pressure << std::endl;
        }
      }
      exit(1);
      particle.position = glm::vec3(0.0f);
    }
    if(glm::any(glm::isnan(particle.velocity))) {
      std::cerr << "Particle velocity is NaN!" << std::endl;
      exit(1);
      particle.velocity = glm::vec3(0.0f);
    }
  }
  return new_particles;
}

#endif
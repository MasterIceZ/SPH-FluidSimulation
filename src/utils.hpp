#ifndef __UTILS_HPP_INCLUDED__
#define __UTILS_HPP_INCLUDED__

#include <iostream>
#include <vector>

#include "types/particle.hpp"

void show_particles(const std::vector<particle_t> &particles) {
  for(const auto &particle: particles) {
    std::cout << "Particle position: (" << particle.position.x << ", "
              << particle.position.y << ", " << particle.position.z << ")"
              << std::endl;
    std::cout << "Particle velocity: (" << particle.velocity.x << ", "
              << particle.velocity.y << ", " << particle.velocity.z << ")"
              << std::endl;
    std::cout << "Particle force: (" << particle.force.x << ", "
              << particle.force.y << ", " << particle.force.z << ")"
              << std::endl;
  }
  std::cout << "----------------------------------------" << std::endl;
}

std::vector<particle_t> normalize_particle(const std::vector<particle_t> &particles, float max_range) {
  std::vector<particle_t> normalized_particles;

  glm::vec3 range = glm::vec3(20, 20, 20);

  for(auto particle: particles) {
    particle.position.x = ((particle.position.x - border_min.x) / range.x) * 2.0f - 1.0f;
    particle.position.y = ((particle.position.y - border_min.y) / range.y) * 2.0f - 1.0f;
    particle.position.z = ((particle.position.z - border_min.z) / range.z) * 2.0f - 1.0f;

    normalized_particles.push_back(particle);
  }

  return normalized_particles;
}

std::vector<glm::vec3> generate_particle_positions(const std::vector<particle_t> &particles) {
  std::vector<glm::vec3> positions;
  for(const auto &particle: particles) {
    positions.push_back(particle.position);
  }
  return positions;
}

void generate_random_particle(std::vector<particle_t> &particles, float min_p, float max_p, int count) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<float> dist(min_p, max_p);

  for(int i = 0; i < count; ++i) {
    float x = dist(rng);
    float y = dist(rng);
    float z = dist(rng);
    particles.push_back({
      glm::vec3(x, y, z), 
      glm::vec3(0), 
      glm::vec3(-1), 
      0.0f, 
      0.0f
    });
  }
}

#endif
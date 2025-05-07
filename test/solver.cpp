#include "../src/solver/solver.hpp"
#include <iostream>

// general settings
float smooth_length = 1.0f;
float time_step = 0.01f;
float gravity = 9.81f;
const glm::vec3 border_min = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 border_max = glm::vec3(10.0f, 10.0f, 10.0f);
const float damp = 1.0f;

// particle properties
float mass = 1.0f;
float rest_density = 1000.0f;
float sound_speed = 1500.0f;
float particle_gamma = 7.0f;

signed main(int argc, char *argv[]) {
  // Test the calculate_density_and_pressure function
  std::vector<particle_t> particles = {
    {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1000.0f, 1.0f},
    {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1000.0f, 1.0f},
    {glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1000.0f, 1.0f}
  };

  auto new_particles = sph_solver(particles);

  for (const auto &particle : new_particles) {
    std::cout << "Particle:\n";
    std::cout << "Density: " << particle.density << ", Pressure: " << particle.pressure << "\n";
    std::cout << "Position: (" << particle.position.x << ", " << particle.position.y << ", " << particle.position.z << ")\n";
    std::cout << "Velocity: (" << particle.velocity.x << ", " << particle.velocity.y << ", " << particle.velocity.z << ")\n";
    std::cout << "Force: (" << particle.force.x << ", " << particle.force.y << ", " << particle.force.z << ")\n";
    std::cout << "------------------------\n";
  }

  return 0;
}


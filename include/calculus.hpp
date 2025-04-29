#ifndef __CALCULUS_HPP_INCLUDED__
#define __CALCULUS_HPP_INCLUDED__

#include <vector>

#include <particle.hpp>

extern double INTEGRATION_TIME_STEP;
extern double EPS;
extern double BOUND_DAMPING;
extern double VIEW_WIDTH;

// Update by integration each time step
particle_t integrate(particle_t particle) {
  particle_t new_particle(particle);
  new_particle.v = new_particle.v + ((new_particle.f * INTEGRATION_TIME_STEP) / new_particle.rho);
  new_particle.position = new_particle.position + (new_particle.v * INTEGRATION_TIME_STEP);

  // Boundary conditions
  if(new_particle.position[0] - EPS < 0.0) {
    new_particle.v[0] = new_particle.v[0] * BOUND_DAMPING;
    new_particle.position[0] = EPS;
  }
  if(new_particle.position[0] + EPS > VIEW_WIDTH) {
    new_particle.v[0] = new_particle.v[0] * BOUND_DAMPING;
    new_particle.position[0] = VIEW_WIDTH - EPS;
  }
  if(new_particle.position[1] - EPS < 0.0) {
    new_particle.v[1] = new_particle.v[1] * BOUND_DAMPING;
    new_particle.position[1] = EPS;
  }
  if(new_particle.position[1] + EPS > VIEW_WIDTH) {
    new_particle.v[1] = new_particle.v[1] * BOUND_DAMPING;
    new_particle.position[1] = VIEW_WIDTH - EPS;
  }
  return new_particle;
}

#endif

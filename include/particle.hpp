#ifndef __PARTICLE_HPP_INCLUDED__
#define __PARTICLE_HPP_INCLUDED__

#include <vec.hpp>

// Particle Data Structure
class particle_t {
public:
  vec<2> position, v, f;
  double rho, p;

  // contructor
  particle_t(double _x, double _y):
    position({_x, _y}), v({0, 0}), f({0, 0}), rho(0), p(0) {}
  particle_t(const particle_t &p):
    position(p.position), v(p.v), f(p.f), rho(p.rho), p(p.p) {}
};

#endif
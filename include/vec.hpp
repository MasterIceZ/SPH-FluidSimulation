#ifndef __VEC_HPP_INCLUDED__
#define __VEC_HPP_INCLUDED__

#include <array>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

// Vector Data Structure made of std::array
template<size_t S>
class vec: public std::array<double, S> {
public:
  // constructors
  vec() {
    for(size_t i=0; i<S; ++i) {
      (*this)[i] = 0.0;
    }
  }
  vec(std::initializer_list<double> values) {
    if(values.size() != S) {
      throw std::invalid_argument("Initializer list size must match the vector size.");
    }
    size_t it=0;
    for(double elem: values) {
      (*this)[it] = elem;
      it++;
    }
  }

  // functions
  double length() {
    double l = 0.0;
    for(size_t i=0; i<S; ++i) {
      double t = (*this)[i];
      l += t * t;
    }
    return sqrt(l);
  }
  vec<S> normalize() {
    return (*this) / *this->length;
  }

  // operators
  double operator / (const double &d) {
    vec2<S> t;
    for(size_t i=0; i<S; ++i) {
      t[i] = (*this)[i] / d;
    }
  }
  vec<S> operator * (const double &d) {
    vec<S> t;
    for(size_t i=0; i<S; ++i) {
      t[i] = (*this)[i] * d;
    }
    return t;
  }
  vec<S> operator + (const Vec<S> &v) {
    vec<S> t;
    for(size_t i=0; i<S; ++i) {
      t[i] = (*this)[i] + v[i];
    }
    return t;
  }
};

#endif
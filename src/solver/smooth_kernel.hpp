#ifndef __SMOOTH_KERNEL_HPP__
#define __SMOOTH_KERNEL_HPP__

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

extern float smooth_length;

float my_pow(float x, int n){
    float result = 1.0f;
    for(int i = 0; i < n; ++i){
        result *= x;
    }
    return result;
}

float w_poly6(glm::vec3 r){
    return (315.0f / (64.0f * M_PI * my_pow(smooth_length, 9))) * my_pow(my_pow(smooth_length, 2) - glm::length(r), 3);
}

float w_spiky(glm::vec3 r){
    return (-45.0f / (M_PI * my_pow(smooth_length, 6))) * my_pow(smooth_length - glm::length(r), 2);
}

float w_viscosity(glm::vec3 r){
    return (45.0f / (M_PI * my_pow(smooth_length, 6))) * (smooth_length - glm::length(r));
}


#endif
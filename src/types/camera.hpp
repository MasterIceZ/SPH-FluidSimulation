#ifndef __CAMERA_HPP_INCLUDED__
#define __CAMERA_HPP_INCLUDED__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct camera_t {
  glm::vec3 position, target, up, worldUp;
  float yaw, pitch, radius;

  camera_t(glm::vec3 _tar, float _r, float _yaw, float _pitch, glm::vec3 _world_up = glm::vec3(0.0f, 1.0f, 0.0f)): 
    target(_tar), radius(_r), yaw(_yaw), pitch(_pitch),  worldUp(_world_up) {
    updateCameraPosition();
  }

  glm::mat4 get_view_matrix() const {
    return glm::lookAt(position, target, up);
  }

  void updateCameraPosition() {
    float y = glm::radians(yaw);
    float p = glm::radians(pitch);
    position.x = target.x + radius * cos(p) * cos(y);
    position.y = target.y + radius * sin(p);
    position.z = target.z + radius * cos(p) * sin(y);

    glm::vec3 front = glm::normalize(target - position);
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
  }
};

#endif
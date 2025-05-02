#ifndef __CAMERA_HPP_INCLUDED__
#define __CAMERA_HPP_INCLUDED__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3

extern float sensitivity;

struct camera_t {
private:
  glm::vec3 position, target, up;
  float yaw, pitch, radius;

public:
  camera_t(glm::vec3 target, float radius, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)):
    target(target), radius(radius), up(up), yaw(-90.0f), pitch(0.0f) {
    update_position();
  }

  glm::mat4 get_view_matrix() {
    return glm::lookAt(position, target, up);
  }

  void process_mouse_movement(float x_offset, float y_offset) {
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f) {
      pitch = 89.0f;
    }
    if (pitch < -89.0f) { 
      pitch = -89.0f;
    }

    update_position();
  }

  void update_position() {
    position.x = radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    position.y = radius * sin(glm::radians(pitch));
    position.z = radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    position += target;
  }
};

#endif
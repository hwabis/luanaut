#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace luanaut {

struct Transform {
  glm::vec3 position{0};
  glm::quat rotation{1, 0, 0, 0};
  glm::vec3 scale{1};

  [[nodiscard]]
  auto ToMatrix() const -> glm::mat4 {
    glm::mat4 translation = glm::translate(glm::mat4(1), position);
    glm::mat4 rotationMat = glm::mat4_cast(rotation);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1), scale);

    return translation * rotationMat * scaleMat;
  }
};

}  // namespace luanaut

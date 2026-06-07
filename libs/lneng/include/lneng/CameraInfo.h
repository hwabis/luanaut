#pragma once
#include <glm/glm.hpp>

namespace lneng {

struct CameraInfo {
  glm::mat4 transformMat{};
  glm::mat4 projectionMat{};
};

}  // namespace lneng

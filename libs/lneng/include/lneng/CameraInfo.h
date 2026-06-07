#pragma once
#include <glm/glm.hpp>

namespace lneng {

struct CameraInfo {
  glm::mat4 viewMat{};
  float fovDeg{};
  float zNear{};
  float zFar{};
};

}  // namespace lneng

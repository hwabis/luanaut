#include "lneng/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace lneng {

constexpr int defaultZFar = 1000;

Camera::Camera(float fovDeg) : fovDeg_(fovDeg) {}

auto Camera::Draw(SceneInfo& out) -> void {
  out.camera = {
      .viewMat = glm::inverse(GetWorldTransform()),
      .fovDeg = fovDeg_,
      .zNear = 1,
      .zFar = defaultZFar,
  };
}

}  // namespace lneng

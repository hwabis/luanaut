#include "lneng/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace lneng {

Camera::Camera(float fovDeg) : fovDeg_(fovDeg) {}

auto Camera::Draw(SceneInfo& out) -> void {
  out.camera = {
      .viewMat = glm::inverse(GetWorldTransform()),
      .fovDeg = fovDeg_,
      .zNear = 1,
      .zFar = 1000,
  };
}

auto Camera::HandleEvent(const SDL_Event& event) -> bool {
  if (event.type == SDL_EVENT_WINDOW_RESIZED) {
    windowWidth_ = static_cast<float>(event.window.data1);
    windowHeight_ = static_cast<float>(event.window.data2);

    return true;
  }

  return false;
}

}  // namespace lneng

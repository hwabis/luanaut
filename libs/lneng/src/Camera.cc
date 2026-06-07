#include "lneng/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace lneng {

Camera::Camera(float fovDeg) : fovDeg_(fovDeg) {}

auto Camera::Draw(SceneInfo& out) -> void {
  out.camera = {
      .transformMat = glm::inverse(GetWorldTransform()),
      .projectionMat = glm::perspectiveLH_ZO(
          glm::radians(fovDeg_), windowWidth_ / windowHeight_, 1.0F, 1000.0F),
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

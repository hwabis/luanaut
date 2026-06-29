#include "lneng/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

namespace lneng {

constexpr int defaultZNear = 1;
constexpr int defaultZFar = 1000;

Camera::Camera(float fovDeg) : fovDeg_(fovDeg) {}

auto Camera::Draw(SceneInfo& out) -> void {
  glm::mat4 effective = applyShake(GetWorldTransform());
  auto viewMat = glm::inverse(effective);

  out.camera = {
      .viewMat = viewMat,
      .fovDeg = fovDeg_,
      .zNear = defaultZNear,
      .zFar = defaultZFar,
  };
}

auto Camera::applyShake(glm::mat4 transform) -> glm::mat4 {
  constexpr float shakeSpeed = 1.5F;
  constexpr glm::vec3 shakeAmplitude{10.0F, 10.0F, 0.0F};
  glm::vec3 shakeOffset{};

  shakeTime_ = clock_->ElapsedSec() * shakeSpeed;

  constexpr int xSeed = 111;
  constexpr int ySeed = 222;
  constexpr int zSeed = 333;
  shakeOffset.x = shakeAmplitude.x * glm::perlin(glm::vec2(xSeed, shakeTime_));
  shakeOffset.y = shakeAmplitude.y * glm::perlin(glm::vec2(ySeed, shakeTime_));
  shakeOffset.z = shakeAmplitude.z * glm::perlin(glm::vec2(zSeed, shakeTime_));

  return glm::translate(transform, shakeOffset);
}

}  // namespace lneng

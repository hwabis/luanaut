#include "lneng/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

namespace lneng {

constexpr int defaultZNear = 1;
constexpr int defaultZFar = 10000;

Camera::Camera(float fovDeg) : fovDeg_(fovDeg) {}

auto Camera::Draw(SceneInfo& out) -> void {
  glm::mat4 worldTransform = GetWorldTransform();

  if (shake_) {
    worldTransform = applyShake(worldTransform);
  }

  out.camera = {
      .viewMat = glm::inverse(worldTransform),
      .fovDeg = fovDeg_,
      .zNear = defaultZNear,
      .zFar = defaultZFar,
  };
}

auto Camera::SetShake(bool shake, glm::vec3 amplitude) -> void {
  shake_ = shake;
  shakeAmplitude_ = amplitude;
}

auto Camera::applyShake(glm::mat4 transform) -> glm::mat4 {
  constexpr float shakeSpeed = 1.5F;
  auto shakeTime_ = clock_->ElapsedSec() * shakeSpeed;

  constexpr int xSeed = 111;
  constexpr int ySeed = 222;
  constexpr int zSeed = 333;

  glm::vec3 shakeOffset{
      shakeAmplitude_.x * glm::perlin(glm::vec2(xSeed, shakeTime_)),
      shakeAmplitude_.y * glm::perlin(glm::vec2(ySeed, shakeTime_)),
      shakeAmplitude_.z * glm::perlin(glm::vec2(zSeed, shakeTime_)),
  };

  return glm::translate(transform, shakeOffset);
}

}  // namespace lneng

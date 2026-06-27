#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include "Transform.h"
#include "Tween.h"

namespace lneng {

class Transformable {
 public:
  virtual auto GetTransform() -> Transform& = 0;

  auto MoveTo(glm::vec3 target, std::chrono::milliseconds duration)
      -> Transformable&;
  auto ScaleTo(glm::vec3 target, std::chrono::milliseconds duration)
      -> Transformable&;
  auto RotateTo(glm::quat target, std::chrono::milliseconds duration)
      -> Transformable&;
  auto Delay(std::chrono::milliseconds duration) -> Transformable&;
  auto Then() -> Transformable&;

  auto UpdateTransforms(std::chrono::steady_clock::time_point now) -> void;

 private:
  std::vector<std::unique_ptr<ATween>> tweens_;
  std::chrono::steady_clock::time_point cursor_;
  std::chrono::milliseconds lastGroupDuration_{};
};

}  // namespace lneng

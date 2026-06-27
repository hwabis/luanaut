#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include "Transform.h"
#include "Tween.h"

namespace lneng {

class Transformable {
 public:
  [[nodiscard]] virtual auto GetTransform() -> Transform& = 0;
  [[nodiscard]] virtual auto GetNow()
      -> std::chrono::steady_clock::time_point = 0;

  // todo come up with a way to have timed/scheduled scene node
  // additions/removals so we dont have to hacky scale to 0 lol

  auto MoveTo(glm::vec3 target, std::chrono::milliseconds duration)
      -> Transformable&;
  auto ScaleTo(glm::vec3 target, std::chrono::milliseconds duration)
      -> Transformable&;
  auto RotateTo(glm::quat target, std::chrono::milliseconds duration)
      -> Transformable&;
  auto RotateTo(float degrees,
                glm::vec3 axis,
                std::chrono::milliseconds duration) -> Transformable&;
  auto Delay(std::chrono::milliseconds duration) -> Transformable&;
  auto Then() -> Transformable&;

  auto UpdateTransforms(std::chrono::steady_clock::time_point now) -> void;

 private:
  auto ensureCursor() -> void;

  std::vector<std::unique_ptr<ATween>> tweens_;
  std::optional<std::chrono::steady_clock::time_point> cursor_;
  std::chrono::milliseconds lastGroupDuration_{};

  std::optional<glm::vec3> latestPos_;
  std::optional<glm::vec3> latestScale_;
  std::optional<glm::quat> latestRot_;
};

}  // namespace lneng

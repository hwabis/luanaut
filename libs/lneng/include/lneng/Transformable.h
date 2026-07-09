#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include "Easings.h"
#include "Transform.h"
#include "Tween.h"

namespace lneng {

using namespace std::chrono_literals;

class Transformable {
 public:
  [[nodiscard]] virtual auto GetTransform() -> Transform& = 0;
  [[nodiscard]] virtual auto GetNow()
      -> std::chrono::steady_clock::time_point = 0;

  auto MoveTo(glm::vec3 target,
              std::chrono::milliseconds duration = 0ms,
              std::function<double(double)> easingFunc = easeLinear)
      -> Transformable&;
  auto ScaleTo(glm::vec3 target,
               std::chrono::milliseconds duration = 0ms,
               std::function<double(double)> easingFunc = easeLinear)
      -> Transformable&;
  auto RotateTo(glm::quat target,
                std::chrono::milliseconds duration = 0ms,
                std::function<double(double)> easingFunc = easeLinear)
      -> Transformable&;
  auto RotateTo(float degrees,
                glm::vec3 axis,
                std::chrono::milliseconds duration = 0ms,
                std::function<double(double)> easingFunc = easeLinear)
      -> Transformable&;
  auto RotateBy(float degrees,
                glm::vec3 axis,
                std::chrono::milliseconds duration = 0ms,
                std::function<double(double)> easingFunc = easeLinear)
      -> Transformable&;
  auto Delay(std::chrono::milliseconds duration) -> Transformable&;
  auto Then() -> Transformable&;

  auto UpdateTweens(std::chrono::steady_clock::time_point now) -> void;
  auto ClearTweens() -> void;

 private:
  auto ensureCursor() -> void;

  std::vector<std::unique_ptr<ATween>> tweens_;
  std::optional<std::chrono::steady_clock::time_point> cursor_;
  std::chrono::milliseconds currentGroupDuration_{};

  std::optional<glm::vec3> latestPos_;
  std::optional<glm::vec3> latestScale_;
  std::optional<glm::quat> latestRot_;
};

}  // namespace lneng

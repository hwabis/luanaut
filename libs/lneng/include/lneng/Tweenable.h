#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include "Easings.h"
#include "Transform.h"
#include "Tween.h"

namespace lneng {

using namespace std::chrono_literals;

class Tweenable {
 public:
  [[nodiscard]] virtual auto GetTransform() -> Transform& = 0;
  [[nodiscard]] virtual auto GetAlpha() -> float& = 0;
  [[nodiscard]] virtual auto GetNow()
      -> std::chrono::steady_clock::time_point = 0;

  auto MoveTo(glm::vec3 target,
              std::chrono::milliseconds duration = 0ms,
              std::function<double(double)> easingFunc = easeLinear)
      -> Tweenable&;
  auto ScaleTo(glm::vec3 target,
               std::chrono::milliseconds duration = 0ms,
               std::function<double(double)> easingFunc = easeLinear)
      -> Tweenable&;
  auto RotateTo(glm::quat target,
                std::chrono::milliseconds duration = 0ms,
                std::function<double(double)> easingFunc = easeLinear)
      -> Tweenable&;
  auto RotateTo(float degrees,
                glm::vec3 axis,
                std::chrono::milliseconds duration = 0ms,
                std::function<double(double)> easingFunc = easeLinear)
      -> Tweenable&;
  auto RotateBy(float degrees,
                glm::vec3 axis,
                std::chrono::milliseconds duration = 0ms,
                std::function<double(double)> easingFunc = easeLinear)
      -> Tweenable&;
  auto FadeTo(float target,
              std::chrono::milliseconds duration = 0ms,
              std::function<double(double)> easingFunc = lneng::easeLinear)
      -> Tweenable&;
  auto Delay(std::chrono::milliseconds duration) -> Tweenable&;
  auto Then() -> Tweenable&;

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
  std::optional<float> latestAlpha_;
};

}  // namespace lneng

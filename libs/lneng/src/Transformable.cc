#include "lneng/Transformable.h"

namespace lneng {

using namespace std::chrono_literals;

auto Transformable::MoveTo(glm::vec3 /*target*/,
                           std::chrono::milliseconds /*duration*/)
    -> Transformable& {
  return *this;
}

auto Transformable::ScaleTo(glm::vec3 /*target*/,
                            std::chrono::milliseconds /*duration*/)
    -> Transformable& {
  return *this;
}

auto Transformable::RotateTo(glm::quat /*target*/,
                             std::chrono::milliseconds /*duration*/)
    -> Transformable& {
  return *this;
}

auto Transformable::Delay(std::chrono::milliseconds /*duration*/)
    -> Transformable& {
  return *this;
}

auto Transformable::Then() -> Transformable& {
  cursor_ += lastGroupDuration_;
  lastGroupDuration_ = 0ms;
  return *this;
}

auto Transformable::UpdateTransforms(std::chrono::steady_clock::time_point now)
    -> void {
  for (auto& tween : tweens_) {
    if (tween->IsActive(now) || tween->IsComplete(now)) {
      tween->Apply(now);
    }
  }
  std::erase_if(tweens_,
                [now](const auto& tween) { return tween->IsComplete(now); });
}

}  // namespace lneng

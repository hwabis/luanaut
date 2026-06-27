#include "lneng/Transformable.h"

namespace lneng {

using namespace std::chrono_literals;

auto Transformable::MoveTo(glm::vec3 target, std::chrono::milliseconds duration)
    -> Transformable& {
  ensureCursor();
  if (!latestPos_.has_value()) {
    latestPos_ = GetTransform().position;
  }

  tweens_.push_back(std::make_unique<Vec3Tween>(
      *cursor_, *cursor_ + duration, *latestPos_, target,
      [this](glm::vec3 current) { GetTransform().position = current; }));

  latestPos_ = target;
  lastGroupDuration_ = std::max(duration, lastGroupDuration_);

  return *this;
}

auto Transformable::ScaleTo(glm::vec3 /*target*/,
                            std::chrono::milliseconds /*duration*/)
    -> Transformable& {
  ensureCursor();

  return *this;
}

auto Transformable::RotateTo(glm::quat /*target*/,
                             std::chrono::milliseconds /*duration*/)
    -> Transformable& {
  ensureCursor();

  return *this;
}

auto Transformable::Delay(std::chrono::milliseconds /*duration*/)
    -> Transformable& {
  ensureCursor();

  return *this;
}

auto Transformable::Then() -> Transformable& {
  *cursor_ += lastGroupDuration_;
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

auto Transformable::ensureCursor() -> void {
  if (!cursor_.has_value()) {
    cursor_ = GetNow();
  }
}

}  // namespace lneng

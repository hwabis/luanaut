#include "lneng/Transformable.h"

namespace lneng {

using namespace std::chrono_literals;

auto Transformable::MoveTo(glm::vec3 target,
                           std::chrono::milliseconds duration,
                           std::function<double(double)> easingFunc)
    -> Transformable& {
  ensureCursor();
  if (!latestPos_.has_value()) {
    latestPos_ = GetTransform().position;
  }

  tweens_.push_back(std::make_unique<Vec3Tween>(
      *cursor_, *cursor_ + duration, *latestPos_, target,
      [this](glm::vec3 current) { GetTransform().position = current; },
      std::move(easingFunc)));

  latestPos_ = target;
  lastGroupDuration_ = std::max(duration, lastGroupDuration_);

  return *this;
}

auto Transformable::ScaleTo(glm::vec3 target,
                            std::chrono::milliseconds duration,
                            std::function<double(double)> easingFunc)
    -> Transformable& {
  ensureCursor();
  if (!latestScale_.has_value()) {
    latestScale_ = GetTransform().scale;
  }

  tweens_.push_back(std::make_unique<Vec3Tween>(
      *cursor_, *cursor_ + duration, *latestScale_, target,
      [this](glm::vec3 current) { GetTransform().scale = current; },
      std::move(easingFunc)));

  latestScale_ = target;
  lastGroupDuration_ = std::max(duration, lastGroupDuration_);

  return *this;
}

auto Transformable::RotateTo(glm::quat target,
                             std::chrono::milliseconds duration,
                             std::function<double(double)> easingFunc)
    -> Transformable& {
  ensureCursor();
  if (!latestRot_.has_value()) {
    latestRot_ = GetTransform().rotation;
  }

  tweens_.push_back(std::make_unique<QuatTween>(
      *cursor_, *cursor_ + duration, *latestRot_, target,
      [this](glm::quat current) { GetTransform().rotation = current; },
      std::move(easingFunc)));

  latestRot_ = target;
  lastGroupDuration_ = std::max(duration, lastGroupDuration_);

  return *this;
}

auto Transformable::RotateTo(float degrees,
                             glm::vec3 axis,
                             std::chrono::milliseconds duration,
                             std::function<double(double)> easingFunc)
    -> Transformable& {
  return RotateTo(glm::angleAxis(glm::radians(degrees), glm::normalize(axis)),
                  duration, std::move(easingFunc));
}

auto Transformable::Delay(std::chrono::milliseconds duration)
    -> Transformable& {
  ensureCursor();
  *cursor_ += lastGroupDuration_ + duration;
  lastGroupDuration_ = 0ms;
  return *this;
}

auto Transformable::Then() -> Transformable& {
  return Delay(0ms);
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

#include "lneng/Tweenable.h"

namespace lneng {

using namespace std::chrono_literals;

auto Tweenable::MoveTo(glm::vec3 target,
                       std::chrono::milliseconds duration,
                       std::function<double(double)> easingFunc) -> Tweenable& {
  ensureCursor();
  if (!latestPos_.has_value()) {
    latestPos_ = GetTransform().position;
  }

  tweens_.push_back(std::make_unique<Vec3Tween>(
      *cursor_, *cursor_ + duration, *latestPos_, target,
      [this](glm::vec3 current) { GetTransform().position = current; },
      std::move(easingFunc)));

  latestPos_ = target;
  currentGroupDuration_ = std::max(duration, currentGroupDuration_);

  return *this;
}

auto Tweenable::ScaleTo(glm::vec3 target,
                        std::chrono::milliseconds duration,
                        std::function<double(double)> easingFunc)
    -> Tweenable& {
  ensureCursor();
  if (!latestScale_.has_value()) {
    latestScale_ = GetTransform().scale;
  }

  tweens_.push_back(std::make_unique<Vec3Tween>(
      *cursor_, *cursor_ + duration, *latestScale_, target,
      [this](glm::vec3 current) { GetTransform().scale = current; },
      std::move(easingFunc)));

  latestScale_ = target;
  currentGroupDuration_ = std::max(duration, currentGroupDuration_);

  return *this;
}

auto Tweenable::RotateTo(glm::quat target,
                         std::chrono::milliseconds duration,
                         std::function<double(double)> easingFunc)
    -> Tweenable& {
  ensureCursor();
  if (!latestRot_.has_value()) {
    latestRot_ = GetTransform().rotation;
  }

  tweens_.push_back(std::make_unique<QuatTween>(
      *cursor_, *cursor_ + duration, *latestRot_, target,
      [this](glm::quat current) { GetTransform().rotation = current; },
      std::move(easingFunc)));

  latestRot_ = target;
  currentGroupDuration_ = std::max(duration, currentGroupDuration_);

  return *this;
}

auto Tweenable::RotateTo(float degrees,
                         glm::vec3 axis,
                         std::chrono::milliseconds duration,
                         std::function<double(double)> easingFunc)
    -> Tweenable& {
  return RotateTo(glm::angleAxis(glm::radians(degrees), glm::normalize(axis)),
                  duration, std::move(easingFunc));
}

auto Tweenable::RotateBy(float degrees,
                         glm::vec3 axis,
                         std::chrono::milliseconds duration,
                         std::function<double(double)> easingFunc)
    -> Tweenable& {
  glm::quat delta = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
  glm::quat start = latestRot_.value_or(GetTransform().rotation);
  return RotateTo(start * delta, duration, std::move(easingFunc));
}

auto Tweenable::Delay(std::chrono::milliseconds duration) -> Tweenable& {
  ensureCursor();
  *cursor_ += currentGroupDuration_ + duration;
  currentGroupDuration_ = 0ms;
  return *this;
}

auto Tweenable::Then() -> Tweenable& {
  return Delay(0ms);
}

auto Tweenable::UpdateTweens(std::chrono::steady_clock::time_point now)
    -> void {
  for (auto& tween : tweens_) {
    if (tween->IsActive(now) || tween->IsComplete(now)) {
      tween->Apply(now);
    }
  }
  std::erase_if(tweens_,
                [now](const auto& tween) { return tween->IsComplete(now); });
}

auto Tweenable::ClearTweens() -> void {
  tweens_.clear();
}

auto Tweenable::ensureCursor() -> void {
  auto now = GetNow();
  if (!cursor_.has_value() || *cursor_ < now) {
    cursor_ = now;
    currentGroupDuration_ = 0ms;
    latestPos_.reset();
    latestScale_.reset();
    latestRot_.reset();
  }
}

}  // namespace lneng

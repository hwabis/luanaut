#pragma once
#include <chrono>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace lneng {

class ATween {
 public:
  ATween(std::chrono::steady_clock::time_point startTime,
         std::chrono::steady_clock::time_point endTime)
      : startTime_(startTime), endTime_(endTime) {}
  virtual ~ATween() = default;

  virtual auto Apply(std::chrono::steady_clock::time_point now) -> void = 0;

  [[nodiscard]] auto IsActive(std::chrono::steady_clock::time_point now) const
      -> bool {
    return now >= startTime_ && now < endTime_;
  }

  [[nodiscard]] auto IsComplete(std::chrono::steady_clock::time_point now) const
      -> bool {
    return now >= endTime_;
  }

 protected:
  std::chrono::steady_clock::time_point startTime_;
  std::chrono::steady_clock::time_point endTime_;
};

template <typename T>
class Tween : public ATween {
 public:
  // todo  easing
  Tween(std::chrono::steady_clock::time_point startTime,
        std::chrono::steady_clock::time_point endTime,
        T startValue,
        T endValue,
        std::function<void(T)> applyFunc)
      : ATween(startTime, endTime),
        startValue_(startValue),
        endValue_(endValue),
        applyFunc_(applyFunc) {}

  auto Apply(std::chrono::steady_clock::time_point now) -> void override {
    if (now <= startTime_) {
      applyFunc_(startValue_);
      return;
    }
    if (now >= endTime_) {
      applyFunc_(endValue_);
      return;
    }

    auto elapsed = (now - startTime_).count();
    auto totalDuration = (endTime_ - startTime_).count();

    double progress =
        static_cast<double>(elapsed) / static_cast<double>(totalDuration);

    // todo how handle easing? and apparently rotation need slerp?
    applyFunc_(Interpolate(startValue_, endValue_, progress));
  }

 protected:
  virtual auto Interpolate(T startValue, T endValue, double progress) -> T = 0;

 private:
  T startValue_;
  T endValue_;
  std::function<void(T)> applyFunc_;
};

class Vec3Tween : public Tween<glm::vec3> {
 public:
  using Tween<glm::vec3>::Tween;

 protected:
  auto Interpolate(glm::vec3 startValue, glm::vec3 endValue, double progress)
      -> glm::vec3 override {
    return glm::mix(startValue, endValue, static_cast<float>(progress));
  }
};

class QuatTween : public Tween<glm::quat> {
 public:
  using Tween<glm::quat>::Tween;

 protected:
  auto Interpolate(glm::quat startValue, glm::quat endValue, double progress)
      -> glm::quat override {
    return glm::slerp(startValue, endValue, static_cast<float>(progress));
  }
};

}  // namespace lneng

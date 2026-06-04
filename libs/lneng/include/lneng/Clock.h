#pragma once
#include <SDL3/SDL.h>
#include <chrono>

namespace lneng {

struct Clock {
  std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
  std::chrono::duration<float, std::milli> deltaTimeMs{0.0F};

  auto Update() -> void {
    auto lastFrameTime = now;
    now = std::chrono::steady_clock::now();
    deltaTimeMs = now - lastFrameTime;
  }
};

}  // namespace lneng

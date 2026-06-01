#pragma once
#include <SDL3/SDL.h>
#include <chrono>

namespace lneng {

struct Clock {
  std::chrono::milliseconds currentTime =
      std::chrono::milliseconds(SDL_GetTicks());
  std::chrono::milliseconds deltaTime{};

  auto Update() -> void {
    auto now = static_cast<std::chrono::milliseconds>(SDL_GetTicks());
    deltaTime = now - currentTime;
    currentTime = now;
  }
};

}  // namespace lneng

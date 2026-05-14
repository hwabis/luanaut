#include "LuanautGame.h"
#include <SDL3/SDL_events.h>

namespace luanaut {

auto LuanautGame::Stop() -> void {
  isRunning_ = false;
}

auto LuanautGame::HandleEvent(const SDL_Event& event) -> bool {
  if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    isRunning_ = false;
    return true;
  }

  return false;
}

}  // namespace luanaut

#include "Game.h"
#include <SDL3/SDL_gpu.h>
#include "DependencyContainer.h"
#include "ResourceManager.h"

namespace luanaut {

Game::Game() {
  deps_ = std::make_unique<DependencyContainer>();
  deps_->Cache(std::make_shared<ResourceManager>());
}

auto Game::IsRunning() const -> bool {
  return isRunning_;
}

auto Game::Update() -> void {
  std::vector<DrawInfo> drawInfos;
  DrawSubTree(drawInfos);
  renderer_->Draw(drawInfos);
}

auto Game::HandleEvent(const SDL_Event& event) -> bool {
  if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    isRunning_ = false;
    return true;
  }

  return false;
}

}  // namespace luanaut

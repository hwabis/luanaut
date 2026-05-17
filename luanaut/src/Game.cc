#include "Game.h"
#include <SDL3/SDL_gpu.h>

namespace luanaut {

Game::Game(std::unique_ptr<Node> root) : root_(std::move(root)) {}

auto Game::HandleEvent(const SDL_Event& event) -> void {
  if (root_->HandleEventSubTree(event)) {
    return;
  }

  if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    isRunning_ = false;
  }
}

auto Game::Update() -> void {
  root_->UpdateSubTree();

  std::vector<DrawInfo> drawInfos;
  root_->DrawSubTree(drawInfos);
  renderer_->Draw(drawInfos);
}

auto Game::SetScene(std::unique_ptr<Node> root) -> void {
  root_ = std::move(root);
}

auto Game::IsRunning() const -> bool {
  return isRunning_;
}

}  // namespace luanaut

#include "Game.h"
#include <spdlog/spdlog.h>

namespace luanaut {

Game::Game(std::unique_ptr<Node> root)
    : root_(std::move(root)),
      window_(SDL_CreateWindow("Luanaut", 0, 0, SDL_WINDOW_FULLSCREEN)),
      device_(SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr)) {
  if (window_ == nullptr) {
    isRunning_ = false;
    spdlog::error(SDL_GetError());
    return;
  }

  if (device_ == nullptr) {
    isRunning_ = false;
    spdlog::error(SDL_GetError());
    return;
  }

  SDL_ClaimWindowForGPUDevice(device_, window_);
}

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
  // todo actually draw everything
}

auto Game::SetScene(std::unique_ptr<Node> root) -> void {
  root_ = std::move(root);
}

auto Game::IsRunning() const -> bool {
  return isRunning_;
}

}  // namespace luanaut

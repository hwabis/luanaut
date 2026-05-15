#include "Game.h"
#include <spdlog/spdlog.h>

namespace luanaut {

Game::Game(std::unique_ptr<Node> root) : root_(std::move(root)) {
  int numDisplays = 0;
  SDL_DisplayID* displays = SDL_GetDisplays(&numDisplays);
  if (numDisplays > 0) {
    const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displays[0]);
    if (mode == nullptr) {
      isRunning_ = false;
      spdlog::error(SDL_GetError());
    } else {
      window_ =
          SDL_CreateWindow("Luanaut", mode->w, mode->h, SDL_WINDOW_RESIZABLE);
      if (window_ == nullptr) {
        isRunning_ = false;
        spdlog::error(SDL_GetError());
      }
    }
  }
  SDL_free(displays);
}

Game::~Game() {
  SDL_DestroyWindow(window_);
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

  std::vector<DrawNode> drawNodes;
  root_->DrawSubTree(drawNodes);
  // todo actually draw everything
}

auto Game::SetScene(std::unique_ptr<Node> root) -> void {
  root_ = std::move(root);
}

auto Game::IsRunning() const -> bool {
  return isRunning_;
}

}  // namespace luanaut

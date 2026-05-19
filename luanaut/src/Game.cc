#include "Game.h"
#include <SDL3/SDL_gpu.h>
#include "DependencyContainer.h"
#include "GpuResourceManager.h"

namespace luanaut {

Game::Game(std::unique_ptr<Scene> initialScene) {
  deps_ = std::make_unique<DependencyContainer>();
  deps_->Cache(std::make_shared<GpuResourceManager>(renderer_->GetWindow(),
                                                    renderer_->GetDevice()));

  SwitchScene(std::move(initialScene));
}

Game::~Game() {
  deps_.reset();  // Destroy before ~renderer_
}

auto Game::SwitchScene(std::unique_ptr<Scene> newScene) -> void {
  if (currentScene_ != nullptr) {
    currentScene_->Destroy();
  }
  currentScene_ = newScene.get();
  AddChild(std::move(newScene));
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

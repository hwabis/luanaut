#include "lneng/Game.h"
#include <SDL3/SDL_gpu.h>
#include "lneng/DependencyContainer.h"

namespace lneng {

Game::Game(std::unique_ptr<Scene> initialScene)
    : gpuResourceLoader_(renderer_.GetWindow(), renderer_.GetDevice()) {
  clock_ = &ownedClock_;

  deps_->Cache(&assetLoader_);
  deps_->Cache(&audioLoader_);
  deps_->Cache(&gpuResourceLoader_);

  auto sceneManager = std::make_unique<SceneManager>(std::move(initialScene));
  sceneManager_ = sceneManager.get();
  deps_->Cache(sceneManager_);
  AddChild(std::move(sceneManager));
}

Game::~Game() {
  deps_.reset();  // Destroy before ~renderer_
}

auto Game::Update() -> void {
  clock_->Update();

  SceneInfo sceneInfo;
  DrawSubTree(sceneInfo);
  renderer_.Draw(sceneInfo);
}

auto Game::HandleEvent(const SDL_Event& event) -> bool {
  if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    Destroy();
    return true;
  }

  return false;
}

}  // namespace lneng

#include "lneng/Game.h"
#include <SDL3/SDL_gpu.h>
#include "lneng/DependencyContainer.h"

namespace lneng {

Game::Game(std::unique_ptr<Scene> initialScene)
    : renderer_(std::make_unique<Renderer>()),
      gpuResourceManager_(
          std::make_unique<GpuResourceManager>(renderer_->GetWindow(),
                                               renderer_->GetDevice())) {
  deps_->Cache(gpuResourceManager_.get());

  auto sceneManager = std::make_unique<SceneManager>(std::move(initialScene));
  sceneManager_ = sceneManager.get();
  deps_->Cache(sceneManager_);
  AddChild(std::move(sceneManager));
}

Game::~Game() {
  deps_.reset();  // Destroy before ~renderer_
}

auto Game::Update() -> void {
  std::vector<DrawInfo> drawInfos;
  DrawSubTree(drawInfos);
  renderer_->Draw(drawInfos);
}

auto Game::HandleEvent(const SDL_Event& event) -> bool {
  if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    Destroy();
    return true;
  }

  return false;
}

}  // namespace lneng

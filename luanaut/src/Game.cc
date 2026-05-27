#include "Game.h"
#include <SDL3/SDL_gpu.h>
#include "DependencyContainer.h"
#include "GpuResourceManager.h"
#include "SceneManager.h"

namespace luanaut {

Game::Game(std::unique_ptr<Scene> initialScene)
    : renderer_(std::make_unique<Renderer>()),
      initialScene_(std::move(initialScene)) {
  deps_->Cache(std::make_shared<GpuResourceManager>(renderer_->GetWindow(),
                                                    renderer_->GetDevice()));

  auto sceneManager = std::make_unique<SceneManager>(std::move(initialScene_));
  // there's something really weird here. questions:
  // - should cache only accept shared ptr?
  // - shoudl addchild only accept unique ptr?
  deps_->Cache(std::shared_ptr<SceneManager>(sceneManager.get(), [](auto*) {}));
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

}  // namespace luanaut

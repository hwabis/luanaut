#include "lneng/SceneManager.h"

namespace lneng {

SceneManager::SceneManager(std::unique_ptr<Scene> initialScene)
    : initialScene_(std::move(initialScene)) {}

auto SceneManager::Load() -> void {
  SwitchScene(std::move(initialScene_));
}

auto SceneManager::Update() -> void {
  if (pendingScene_ != nullptr) {
    if (currentScene_ != nullptr) {
      currentScene_->Destroy();
    }

    currentScene_ = pendingScene_.get();
    AddChild(std::move(pendingScene_));
    pendingScene_ = nullptr;
    // todo handle scene transition effects and stuff
  }
}

auto SceneManager::SwitchScene(std::unique_ptr<Scene> newScene) -> void {
  pendingScene_ = std::move(newScene);
}

}  // namespace lneng

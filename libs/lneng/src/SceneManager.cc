#include "lneng/SceneManager.h"

namespace lneng {

SceneManager::SceneManager(std::unique_ptr<Scene> initialScene)
    : initialScene_(std::move(initialScene)) {}

auto SceneManager::Load() -> void {
  SwitchScene(std::move(initialScene_));
}

auto SceneManager::SwitchScene(std::unique_ptr<Scene> newScene) -> void {
  if (currentScene_ != nullptr) {
    currentScene_->Destroy();
  }
  currentScene_ = newScene.get();
  AddChild(std::move(newScene));
  // todo handle scene transition effects and stuff
}

}  // namespace lneng

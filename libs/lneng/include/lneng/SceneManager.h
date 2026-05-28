#pragma once
#include "Node.h"
#include "Scene.h"

namespace lneng {

class SceneManager : public Node {
 public:
  SceneManager(std::unique_ptr<Scene> initialScene);
  auto SwitchScene(std::unique_ptr<Scene> newScene) -> void;

 protected:
  auto Load() -> void override;
  auto Update() -> void override;

 private:
  // Only allow 1 scene as sole child
  using Node::AddChild;
  Scene* currentScene_ = nullptr;

  std::unique_ptr<Scene> initialScene_ = nullptr;
  std::unique_ptr<Scene> pendingScene_ = nullptr;
};

}  // namespace lneng

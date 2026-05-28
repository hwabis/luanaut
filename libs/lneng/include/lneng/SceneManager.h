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

 private:
  // Only allow 1 scene as sole child
  using Node::AddChild;
  Scene* currentScene_ = nullptr;

  // Used as handoff from ctor to load
  std::unique_ptr<Scene> initialScene_;
};

}  // namespace lneng

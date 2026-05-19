#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include "Node.h"
#include "Renderer.h"
#include "Scene.h"

namespace luanaut {

class Game : public Node {
 public:
  Game(std::unique_ptr<Scene> initialScene);
  ~Game() override;

  auto SwitchScene(std::unique_ptr<Scene> scene) -> void;

 protected:
  auto Update() -> void override;
  auto HandleEvent(const SDL_Event& event) -> bool override;

 private:
  // Only allow scene as sole child
  using Node::AddChild;

  Scene* currentScene_ = nullptr;

  std::unique_ptr<Renderer> renderer_ = std::make_unique<Renderer>();
};

}  // namespace luanaut

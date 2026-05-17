#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include "Node.h"
#include "Renderer.h"
#include "ResourceManager.h"

namespace luanaut {

class Game {
 public:
  Game(std::unique_ptr<Node> root);

  auto HandleEvent(const SDL_Event& event) -> void;

  auto Update() -> void;
  auto SetScene(std::unique_ptr<Node> root) -> void;
  [[nodiscard]] auto IsRunning() const -> bool;

 private:
  std::unique_ptr<Node> root_;
  std::unique_ptr<Renderer> renderer_ = std::make_unique<Renderer>();
  std::unique_ptr<ResourceManager> resources_ =
      std::make_unique<ResourceManager>();

  bool isRunning_ = true;
};

}  // namespace luanaut

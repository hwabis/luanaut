#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include "Node.h"
#include "Renderer.h"

namespace luanaut {

class Game : public Node {
 public:
  Game();
  ~Game() override;

  [[nodiscard]] auto IsRunning() const -> bool;

  // todo we need some sort of scene system

 protected:
  auto Update() -> void override;
  auto HandleEvent(const SDL_Event& event) -> bool override;

 private:
  std::unique_ptr<Renderer> renderer_ = std::make_unique<Renderer>();
  bool isRunning_ = true;
};

}  // namespace luanaut

#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include "Node.h"
#include "SdlHandles.h"

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

  SdlWindowHandle window_;
  SdlGpuDeviceHandle device_;

  bool isRunning_ = true;
};

}  // namespace luanaut

#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include "Node.h"

namespace luanaut {

class Game {
 public:
  Game(std::unique_ptr<Node> root);
  ~Game();

  Game(const Game&) = delete;
  auto operator=(const Game&) -> Game& = delete;
  Game(Game&&) = default;
  auto operator=(Game&&) -> Game& = default;

  auto HandleEvent(const SDL_Event& event) -> void;

  auto Update() -> void;
  auto SetScene(std::unique_ptr<Node> root) -> void;
  [[nodiscard]] auto IsRunning() const -> bool;

 private:
  std::unique_ptr<Node> root_;
  SDL_Window* window_ = nullptr;
  bool isRunning_ = true;
};

}  // namespace luanaut

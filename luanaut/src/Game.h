#pragma once
#include <memory>
#include "Node.h"

namespace luanaut {

class Game {
 public:
  Game(std::unique_ptr<Node> root) : root_(std::move(root)) {}

  auto Update() -> void;
  auto Stop() -> void;

 private:
  std::unique_ptr<Node> root_;
  bool isRunning_ = true;
};

}  // namespace luanaut

#pragma once
#include <memory>
#include "Node.h"

namespace luanaut {

class Game : public Node {
 public:
  auto Stop() -> void;
  [[nodiscard]] auto IsRunning() const -> bool { return isRunning_; }

 private:
  bool isRunning_ = true;
};

}  // namespace luanaut

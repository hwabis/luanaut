#pragma once
#include "Node.h"

namespace luanaut {

class LuanautGame : public Node {
 public:
  auto Stop() -> void;
  [[nodiscard]] auto IsRunning() const -> bool { return isRunning_; }

  auto HandleEvent(const SDL_Event& event) -> bool override;

 private:
  bool isRunning_ = true;
};

}  // namespace luanaut

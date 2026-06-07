#pragma once
#include "Node.h"

namespace lneng {

class Camera : public Node {
 public:
  // todo adjustable proj matrix
  Camera(float fovDeg);

 protected:
  auto Draw(SceneInfo& out) -> void override;
  auto HandleEvent(const SDL_Event& event) -> bool override;

 private:
  float fovDeg_{};

  // todo these values are completely made up until HandleEvent is first called
  float windowWidth_ = 1;
  float windowHeight_ = 1;
};

}  // namespace lneng

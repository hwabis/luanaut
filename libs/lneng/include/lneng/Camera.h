#pragma once
#include "Node.h"

namespace lneng {

class Camera : public Node {
 public:
  // todo adjustable proj matrix
  Camera(float fovDeg);
  auto SetShake(bool shake) -> void;

 protected:
  auto Draw(SceneInfo& out) -> void override;

 private:
  auto applyShake(glm::mat4 transform) -> glm::mat4;

  float fovDeg_{};
  float shakeTime_{};
  bool shake_{};
};

}  // namespace lneng

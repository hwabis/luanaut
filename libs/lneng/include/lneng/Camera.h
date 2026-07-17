#pragma once
#include "Node.h"

namespace lneng {

class Camera : public Node {
 public:
  // todo adjustable proj matrix
  Camera(float fovDeg);
  auto SetShake(bool shake, glm::vec3 amplitude = {1, 1, 1}) -> void;

 protected:
  auto Draw(SceneInfo& out) -> void override;

 private:
  auto applyShake(glm::mat4 transform) -> glm::mat4;

  float fovDeg_{};

  bool shake_{};
  glm::vec3 shakeAmplitude_{1, 1, 1};
};

}  // namespace lneng

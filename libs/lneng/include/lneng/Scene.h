#pragma once
#include "Node.h"
#include "Transform.h"

namespace lneng {

class Scene : public Node {
 public:
  Scene(Transform cameraStart, float cameraFov);

  auto Load() -> void final;

  // todo some OnEnter/OnExit transform shenanigans
  // invoke them in game

 protected:
  virtual auto LoadScene() -> void;

 private:
  // Initial fields
  Transform cameraStart_;
  float cameraFov_;
};

}  // namespace lneng

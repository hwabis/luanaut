#pragma once
#include "Node.h"

namespace lneng {

class Camera : public Node {
 public:
  // todo adjustable proj matrix
  Camera(float fovDeg);

 protected:
  auto Draw(SceneInfo& out) -> void override;

 private:
  float fovDeg_{};
};

}  // namespace lneng

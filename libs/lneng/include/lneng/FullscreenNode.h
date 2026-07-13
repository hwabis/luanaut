#pragma once
#include "Node.h"

namespace lneng {

class FullscreenNode : public Node {
 public:
  FullscreenNode(glm::vec3 fadeColor);

 protected:
  auto Load() -> void override;
  auto Draw(SceneInfo& out) -> void override;

 private:
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
  glm::vec3 fadeColor_;
};

}  // namespace lneng

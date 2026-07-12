#pragma once
#include "Node.h"

namespace lneng {

class FullscreenNode : public Node {
 protected:
  auto Load() -> void override;
  auto Draw(SceneInfo& out) -> void override;

 private:
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
  glm::vec3 fadeColor_ = {1, 1, 1};  // todo public way to set
};

}  // namespace lneng

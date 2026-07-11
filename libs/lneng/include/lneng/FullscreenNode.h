#pragma once
#include "Node.h"

namespace lneng {

class FullscreenNode : public Node {
 protected:
  auto Load() -> void override;
  auto Draw(SceneInfo& out) -> void override;

 private:
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
};

}  // namespace lneng

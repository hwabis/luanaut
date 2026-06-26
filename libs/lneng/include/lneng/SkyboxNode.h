#pragma once
#include "Node.h"
#include "Skybox.h"
#include "SkyboxCreateInfo.h"

namespace lneng {

class SkyboxNode : public Node {
 public:
  SkyboxNode(SkyboxCreateInfo info);

 protected:
  auto Load() -> void override;
  auto Draw(SceneInfo& out) -> void override;

 private:
  SkyboxCreateInfo info_;
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
  Skybox* skybox_ = nullptr;
};

}  // namespace lneng

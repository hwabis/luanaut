#pragma once
#include "MaterialUbo.h"
#include "Model.h"
#include "ModelCreateInfo.h"
#include "Node.h"

namespace lneng {

class ModelNode : public Node {
 public:
  ModelNode(ModelCreateInfo modelInfo);

  auto GetMaterial() -> MaterialUbo&;

 protected:
  auto Load() -> void override;
  auto Draw(SceneInfo& out) -> void override;

 private:
  ModelCreateInfo modelInfo_;
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
  Model* model_ = nullptr;
  MaterialUbo material_;
};

}  // namespace lneng

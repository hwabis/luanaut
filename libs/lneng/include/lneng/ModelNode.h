#pragma once
#include "Model.h"
#include "ModelCreateInfo.h"
#include "Node.h"

namespace lneng {

class ModelNode : public Node {
 public:
  ModelNode(ModelCreateInfo modelInfo);

 protected:
  auto Load() -> void override;
  auto Draw(std::vector<DrawInfo>& out) -> void override;

 private:
  ModelCreateInfo modelInfo_;
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
  Model* model_ = nullptr;
};

}  // namespace lneng

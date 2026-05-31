#pragma once
#include "Model.h"
#include "Node.h"

namespace lneng {

class ModelNode : public Node {
 public:
  ModelNode(Model::Creation modelInfo);

 protected:
  auto Load() -> void override;
  auto Draw(std::vector<DrawInfo>& out) -> void override;

 private:
  Model::Creation modelInfo_;
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
  Model* model_ = nullptr;
};

}  // namespace lneng

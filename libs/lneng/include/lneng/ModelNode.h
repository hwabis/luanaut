#pragma once
#include "ModelInfo.h"
#include "Node.h"

namespace lneng {

class ModelNode : public Node {
 public:
  ModelNode(ModelInfo asset);

 protected:
  auto Load() -> void override;
  auto Draw(std::vector<DrawInfo>& out) -> void override;

 private:
  ModelInfo asset_;
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
  Mesh* mesh_ = nullptr;
};

}  // namespace lneng

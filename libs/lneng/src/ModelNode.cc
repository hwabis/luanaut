#include "lneng/ModelNode.h"
#include "lneng/GpuResourceLoader.h"

namespace lneng {

ModelNode::ModelNode(Model::Creation modelInfo)
    : modelInfo_(std::move(modelInfo)) {}

auto ModelNode::Load() -> void {
  auto* gpuLoader = deps_->Resolve<GpuResourceLoader>();
  pipeline_ = gpuLoader->CreateGpuGraphicsPipeline(GpuGraphicsPipelineInfo{
      .vertShaderAttributes = Vertex::GetAttributes(),
      .vertShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.vert.spv",
      .fragShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.frag.spv",
  });
  model_ = gpuLoader->CreateModel(modelInfo_);
}

auto ModelNode::Draw(std::vector<DrawInfo>& out) -> void {
  out.push_back(DrawInfo{
      .worldTransform = GetWorldTransform(),
      .pipeline = pipeline_,
      .model = model_,
  });
}

}  // namespace lneng

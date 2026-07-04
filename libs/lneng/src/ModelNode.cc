#include "lneng/ModelNode.h"
#include "lneng/GpuResourceLoader.h"
#include "lneng/ModelCreateInfo.h"

namespace lneng {

ModelNode::ModelNode(ModelCreateInfo modelInfo)
    : modelInfo_(std::move(modelInfo)) {}

auto ModelNode::GetMaterial() -> MaterialUbo& {
  return material_;
}

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

auto ModelNode::Draw(SceneInfo& out) -> void {
  out.draws.push_back(DrawInfo{
      .worldTransform = GetWorldTransform(),
      .pipeline = pipeline_,
      .model = model_,
      .material = material_,
  });
}

}  // namespace lneng

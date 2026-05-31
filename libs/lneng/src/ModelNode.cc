#include "lneng/ModelNode.h"
#include "lneng/GpuResourceLoader.h"

namespace lneng {

ModelNode::ModelNode(ModelInfo asset) : asset_(std::move(asset)) {}

auto ModelNode::Load() -> void {
  // todo handle if asset has multiple meshes/materials
  // (vector<Mesh*> or something)

  auto* gpuLoader = deps_->Resolve<GpuResourceLoader>();
  pipeline_ = gpuLoader->CreateGpuGraphicsPipeline({
      .vertShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.vert.spv",
      .fragShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.frag.spv",
  });
  mesh_ = gpuLoader->CreateMesh(asset_.meshes[0]);
}

auto ModelNode::Draw(std::vector<DrawInfo>& out) -> void {
  out.push_back(DrawInfo{
      .worldTransform = GetWorldTransform(),
      .pipeline = pipeline_,
      .mesh = mesh_,
  });
}

}  // namespace lneng

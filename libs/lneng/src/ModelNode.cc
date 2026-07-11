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
  // todo this is a little hacky. The only way this condition hits is if it's
  // immediately set after this node is constructed (before Load). Not mid-scene
  if (material_.edgeWidth > 0.0F) {
    for (auto& mesh : modelInfo_.meshes) {
      mesh = ModelCreateInfo::Deindex(mesh);
    }
  }

  SDL_GPUDepthStencilState depthStencilState{};
  depthStencilState.compare_op = SDL_GPU_COMPAREOP_LESS;
  depthStencilState.enable_depth_test = true;
  depthStencilState.enable_depth_write = true;

  auto* gpuLoader = deps_->Resolve<GpuResourceLoader>();
  pipeline_ = gpuLoader->CreateGpuGraphicsPipeline(GpuGraphicsPipelineInfo{
      .vertShaderAttributes = Vertex::GetAttributes(),
      .sizeOfVertex = sizeof(Vertex),
      .vertShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.vert.spv",
      .fragShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.frag.spv",
      .cullMode = SDL_GPU_CULLMODE_BACK,
      .depthStencilState = depthStencilState,
      .enableBlend = false,
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

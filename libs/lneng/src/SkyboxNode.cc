#include "lneng/SkyboxNode.h"
#include "lneng/GpuResourceLoader.h"

namespace lneng {

SkyboxNode::SkyboxNode(SkyboxCreateInfo info) : info_(std::move(info)) {}

auto SkyboxNode::Load() -> void {
  auto* gpuLoader = deps_->Resolve<GpuResourceLoader>();
  pipeline_ = gpuLoader->CreateGpuGraphicsPipeline(GpuGraphicsPipelineInfo{
      .vertShaderAttributes = Vertex::GetAttributes(),
      // TODO skybox shader
      .vertShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.vert.spv",
      .fragShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.frag.spv",
      .isSkybox = true,
  });

  skybox_ = gpuLoader->CreateSkybox(info_);
}

auto SkyboxNode::Draw(SceneInfo& out) -> void {
  out.skybox = {
      .pipeline = pipeline_,
      .skybox = skybox_,
  };
}

}  // namespace lneng

#include "lneng/SkyboxNode.h"
#include <SDL3/SDL_gpu.h>
#include "lneng/GpuResourceLoader.h"
#include "lneng/SkyboxVertex.h"

namespace lneng {

SkyboxNode::SkyboxNode(SkyboxCreateInfo info) : info_(std::move(info)) {}

auto SkyboxNode::Load() -> void {
  auto* gpuLoader = deps_->Resolve<GpuResourceLoader>();

  SDL_GPUDepthStencilState depthStencilState{};
  depthStencilState.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;
  depthStencilState.enable_depth_test = true;
  depthStencilState.enable_depth_write = false;

  pipeline_ = gpuLoader->CreateGpuGraphicsPipeline(GpuGraphicsPipelineInfo{
      .vertShaderAttributes = SkyboxVertex::GetAttributes(),
      .sizeOfVertex = sizeof(SkyboxVertex),
      .vertShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "skybox.vert.spv",
      .fragShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "skybox.frag.spv",
      .cullMode = SDL_GPU_CULLMODE_NONE,
      .depthStencilState = depthStencilState,
      .enableBlend = false,
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

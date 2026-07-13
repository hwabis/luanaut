#include "lneng/FullscreenNode.h"
#include <SDL3/SDL_gpu.h>
#include "lneng/GpuResourceLoader.h"

namespace lneng {

FullscreenNode::FullscreenNode(glm::vec3 fadeColor) : fadeColor_(fadeColor) {}

auto FullscreenNode::Load() -> void {
  auto* gpuLoader = deps_->Resolve<GpuResourceLoader>();

  pipeline_ = gpuLoader->CreateGpuGraphicsPipeline(GpuGraphicsPipelineInfo{
      .vertShaderAttributes = {},
      .sizeOfVertex = 0,
      .vertShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "fullscreen.vert.spv",
      .fragShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "fullscreen.frag.spv",
      .cullMode = SDL_GPU_CULLMODE_NONE,
      .depthStencilState = {},
      .enableBlend = true,
  });
}

auto FullscreenNode::Draw(SceneInfo& out) -> void {
  out.fullscreens.push_back(FullscreenInfo{
      .pipeline = pipeline_,
      .fadeColor = glm::vec4(fadeColor_, GetAlpha()),
  });
}

}  // namespace lneng

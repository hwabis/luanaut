#include "lneng/ParticleNode.h"
#include "lneng/GpuResourceLoader.h"
#include "lneng/ParticleInfo.h"

namespace lneng {

ParticleNode::ParticleNode(glm::vec3 color, float size, float alpha)
    : color_(color), size_(size), alpha_(alpha) {}

auto ParticleNode::Load() -> void {
  auto* gpuLoader = deps_->Resolve<GpuResourceLoader>();

  SDL_GPUDepthStencilState depthStencilState{};
  depthStencilState.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;
  depthStencilState.enable_depth_test = true;
  depthStencilState.enable_depth_write = false;

  pipeline_ = gpuLoader->CreateGpuGraphicsPipeline(GpuGraphicsPipelineInfo{
      .vertShaderAttributes = {},
      .sizeOfVertex = 0,
      .vertShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "particle.vert.spv",
      .fragShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "particle.frag.spv",
      .cullMode = SDL_GPU_CULLMODE_NONE,
      .depthStencilState = depthStencilState,
      .enableBlend = true,
  });
}

auto ParticleNode::Draw(SceneInfo& out) -> void {
  // only care about position
  glm::vec3 worldPos = glm::vec3(GetWorldTransform()[3]);

  out.particles.push_back(ParticleInfo{
      .pipeline = pipeline_,
      .worldPos = worldPos,
      .color = color_,
      .size = size_,
      .alpha = alpha_,
  });
}

}  // namespace lneng

#pragma once
#include <SDL3/SDL_gpu.h>
#include <filesystem>
#include <vector>

namespace lneng {

struct GpuGraphicsPipelineInfo {
  std::vector<SDL_GPUVertexAttribute> vertShaderAttributes;
  Uint32 sizeOfVertex;
  std::filesystem::path vertShaderPath;
  std::filesystem::path fragShaderPath;
  SDL_GPUCullMode cullMode;
  SDL_GPUDepthStencilState depthStencilState{};
  bool enableBlend;

  [[nodiscard]] auto GetHashKey() const -> std::string {
    return vertShaderPath.string() + fragShaderPath.string();
  }
};

}  // namespace lneng

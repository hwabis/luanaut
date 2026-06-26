#pragma once
#include <SDL3/SDL_gpu.h>
#include <filesystem>
#include <vector>

namespace lneng {

struct GpuGraphicsPipelineInfo {
  std::vector<SDL_GPUVertexAttribute> vertShaderAttributes;
  std::filesystem::path vertShaderPath;
  std::filesystem::path fragShaderPath;
  bool isSkybox = false;

  [[nodiscard]] auto GetHashKey() const -> std::string {
    return vertShaderPath.string() + fragShaderPath.string();
  }
};

}  // namespace lneng

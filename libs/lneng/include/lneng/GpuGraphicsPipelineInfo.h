#pragma once
#include <filesystem>

namespace lneng {

struct GpuGraphicsPipelineInfo {
  // todo vertex format should be HERE
  // todo there should probably be no paths below... maybe raw spirv
  std::filesystem::path vertShaderPath;
  std::filesystem::path fragShaderPath;

  [[nodiscard]] auto GetHashKey() const -> std::string {
    return vertShaderPath.string() + fragShaderPath.string();
  }
};

}  // namespace lneng

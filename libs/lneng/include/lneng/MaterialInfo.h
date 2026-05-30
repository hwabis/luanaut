#pragma once
#include <filesystem>
#include <string>

namespace lneng {

struct MaterialInfo {
  // todo there should probably be no paths here... maybe raw spirv
  std::filesystem::path vertShaderPath;
  std::filesystem::path fragShaderPath;
  std::string cacheKey;
  // todo textures etc?
};

}  // namespace lneng

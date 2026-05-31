#pragma once
#include <filesystem>
#include "ModelInfo.h"

namespace lneng {

class AssetLoader {
 public:
  auto LoadGlb(const std::filesystem::path& path) -> ModelInfo;
};

}  // namespace lneng

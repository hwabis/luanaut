#pragma once
#include <filesystem>
#include "ModelCreateInfo.h"

namespace lneng {

class AssetLoader {
 public:
  auto LoadGlb(const std::filesystem::path& path) -> ModelCreateInfo;
};

}  // namespace lneng

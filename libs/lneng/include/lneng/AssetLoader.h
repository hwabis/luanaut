#pragma once
#include <filesystem>
#include "GltfAsset.h"

namespace lneng {

class AssetLoader {
 public:
  auto LoadGlb(const std::filesystem::path& path) -> GltfAsset;
};

}  // namespace lneng

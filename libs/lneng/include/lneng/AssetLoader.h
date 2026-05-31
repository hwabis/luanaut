#pragma once
#include <filesystem>
#include "Model.h"

namespace lneng {

class AssetLoader {
 public:
  auto LoadGlb(const std::filesystem::path& path) -> Model::Creation;
};

}  // namespace lneng

#pragma once
#include <filesystem>
#include "ModelCreateInfo.h"

namespace lneng {

class AssetLoader {
 public:
  auto LoadGlb(const std::filesystem::path& path) -> ModelCreateInfo;

 private:
  static auto decodeImageBytes(const std::byte* data, size_t size) -> Texture;
};

}  // namespace lneng

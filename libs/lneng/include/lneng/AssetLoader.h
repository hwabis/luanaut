#pragma once
#include <filesystem>
#include "ModelCreateInfo.h"
#include "SkyboxCreateInfo.h"

namespace lneng {

class AssetLoader {
 public:
  auto LoadGlb(const std::filesystem::path& path) -> ModelCreateInfo;
  auto LoadSkybox(const std::filesystem::path& xPosTexture,
                  const std::filesystem::path& xNegTexture,
                  const std::filesystem::path& yPosTexture,
                  const std::filesystem::path& yNegTexture,
                  const std::filesystem::path& zPosTexture,
                  const std::filesystem::path& zNegTexture) -> SkyboxCreateInfo;

 private:
  static auto decodeImageBytes(const std::byte* data, size_t size) -> Texture;
  static auto decodeImageFile(const std::filesystem::path& path) -> Texture;
};

}  // namespace lneng

#pragma once
#include <vector>
#include "lneng/SdlHandles.h"

namespace lneng {

struct Model {
  SdlGpuBufferHandle vertexBuffer;
  SdlGpuBufferHandle indexBuffer;
  std::vector<SdlGpuTextureHandle> textures;
  std::vector<SdlGpuSamplerHandle> samplers;

  struct Submesh {
    uint32_t firstIndex;
    uint32_t indexCount;
    uint32_t textureIndex;
  };
  std::vector<Submesh> submeshes;
};

}  // namespace lneng

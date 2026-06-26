#pragma once
#include "lneng/SdlHandles.h"

namespace lneng {

// todo leaky ?
constexpr int indexCount = 36;

struct Skybox {
  SdlGpuTextureHandle texture;

  // Everything below is same for all skyboxes but we just put everything here
  SdlGpuSamplerHandle sampler;
  SdlGpuBufferHandle vertexBuffer;
  SdlGpuBufferHandle indexBuffer;
  uint32_t indexBufferCount = indexCount;
};

}  // namespace lneng

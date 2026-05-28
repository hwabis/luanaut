#pragma once
#include "SdlHandles.h"

namespace lneng {

struct Mesh {
  SdlGpuBufferHandle vertexBuffer;
  SdlGpuBufferHandle indexBuffer;
  uint32_t indexBufferCount{};
};

}  // namespace lneng

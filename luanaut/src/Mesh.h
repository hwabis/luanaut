#pragma once
#include "SdlHandles.h"

namespace luanaut {

struct Mesh {
  SdlGpuBufferHandle vertexBuffer;
  SdlGpuBufferHandle indexBuffer;
  uint32_t indexBufferCount{};
};

}  // namespace luanaut

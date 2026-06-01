#pragma once
#include "lneng/SdlHandles.h"

namespace lneng {

struct Model {
  // todo support multiple mesh like in creation
  SdlGpuBufferHandle vertexBuffer;
  SdlGpuBufferHandle indexBuffer;
  uint32_t indexBufferCount{};

  // todo add texture/sample structs
};

}  // namespace lneng

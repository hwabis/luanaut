#pragma once
#include "DrawInfo.h"
#include "SdlHandles.h"

namespace luanaut {

class Renderer {
 public:
  Renderer();

  auto Draw(const std::vector<DrawInfo>& draws) -> void;

 private:
  SdlWindowHandle window_;
  SdlGpuDeviceHandle device_;
};

}  // namespace luanaut

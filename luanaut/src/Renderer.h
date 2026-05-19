#pragma once
#include "DrawInfo.h"
#include "SdlHandles.h"

namespace luanaut {

class Renderer {
 public:
  Renderer();

  auto Draw(const std::vector<DrawInfo>& draws) -> void;

  [[nodiscard]] auto GetDevice() const -> SDL_GPUDevice*;
  [[nodiscard]] auto GetWindow() const -> SDL_Window*;

 private:
  SdlWindowHandle window_;
  SdlGpuDeviceHandle device_;
};

}  // namespace luanaut

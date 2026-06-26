#pragma once
#include "SceneInfo.h"
#include "SdlHandles.h"

namespace lneng {

class Renderer {
 public:
  Renderer();

  auto Draw(const SceneInfo& scene) -> void;

  [[nodiscard]] auto GetDevice() const -> SDL_GPUDevice*;
  [[nodiscard]] auto GetWindow() const -> SDL_Window*;

 private:
  auto drawSkybox() -> void;

  SdlWindowHandle window_;
  SdlGpuDeviceHandle device_;

  SdlGpuTextureHandle depthTexture_;
  Uint32 depthWidth_ = 0;
  Uint32 depthHeight_ = 0;
};

}  // namespace lneng

#pragma once
#include "SdlHandles.h"
#include "Skybox.h"

namespace lneng {

struct SkyboxInfo {
  SdlGpuGraphicsPipelineHandle* pipeline = nullptr;
  Skybox* skybox = nullptr;
};

}  // namespace lneng

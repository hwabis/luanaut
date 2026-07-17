#pragma once
#include "SdlHandles.h"
#include "Skybox.h"

namespace lneng {

struct SkyboxInfo {
  SdlGpuGraphicsPipelineHandle* pipeline;
  Skybox* skybox;
};

}  // namespace lneng

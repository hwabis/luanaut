#pragma once
#include "SdlHandles.h"
#include "Skybox.h"

namespace lneng {

struct SkyboxInfo {
  // todo since the pipeline is same for all skyboxes this shouldn't be here
  SdlGpuGraphicsPipelineHandle* pipeline;
  Skybox* skybox;
};

}  // namespace lneng

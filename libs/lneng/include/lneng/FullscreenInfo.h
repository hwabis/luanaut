#pragma once
#include <glm/glm.hpp>
#include "SdlHandles.h"

namespace lneng {

struct FullscreenInfo {
  SdlGpuGraphicsPipelineHandle* pipeline;
  glm::vec4 fadeColor;
};

}  // namespace lneng

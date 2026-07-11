#pragma once
#include <glm/glm.hpp>
#include "SdlHandles.h"

namespace lneng {

struct FullscreenInfo {
  SdlGpuGraphicsPipelineHandle* pipeline = nullptr;
  glm::vec4 fadeColor{0, 0, 0, 0};
};

}  // namespace lneng

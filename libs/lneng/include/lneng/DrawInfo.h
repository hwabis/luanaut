#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "Model.h"
#include "SdlHandles.h"

namespace lneng {

struct DrawInfo {
  glm::mat4 worldTransform{1.0F};
  SdlGpuGraphicsPipelineHandle* pipeline = nullptr;
  Model* model = nullptr;
};

}  // namespace lneng

#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "MaterialUbo.h"
#include "Model.h"

namespace lneng {

struct DrawInfo {
  glm::mat4 worldTransform;
  SdlGpuGraphicsPipelineHandle* pipeline;
  Model* model;
  MaterialUbo material;
};

}  // namespace lneng

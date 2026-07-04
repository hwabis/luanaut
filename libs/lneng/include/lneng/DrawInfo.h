#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "MaterialUbo.h"
#include "Model.h"

namespace lneng {

struct DrawInfo {
  glm::mat4 worldTransform{1.0F};
  SdlGpuGraphicsPipelineHandle* pipeline = nullptr;
  Model* model = nullptr;
  MaterialUbo material;
};

}  // namespace lneng

#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "lneng/SdlHandles.h"

namespace lneng {

struct DrawInfo {
  glm::mat4 worldTransform{1.0F};
  SdlGpuGraphicsPipelineHandle* pipeline = nullptr;
  Mesh* mesh = nullptr;
  // todo how do we access texture? keep ModelInfo out of this right?
};

}  // namespace lneng

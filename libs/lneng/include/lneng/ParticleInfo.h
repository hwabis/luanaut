#pragma once
#include <glm/glm.hpp>
#include "SdlHandles.h"

namespace lneng {

struct ParticleInfo {
  // todo since the pipeline is same for all particles this shouldn't be here
  SdlGpuGraphicsPipelineHandle* pipeline;
  glm::vec3 worldPos;
  glm::vec3 color;
  float size;
  float alpha;
};

}  // namespace lneng

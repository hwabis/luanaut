#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "Mesh.h"

namespace luanaut {

struct DrawInfo {
  glm::mat4 worldTransform{1.0F};
  // todo variant-ify this to also handle 2D (would that just be a texture)?
  Mesh* mesh{};
  SDL_GPUGraphicsPipeline* pipeline{};
};

}  // namespace luanaut

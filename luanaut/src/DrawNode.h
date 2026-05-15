#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>

namespace luanaut {

struct DrawNode {
  glm::mat4 worldTransform{1.0F};
  SDL_GPUBuffer* vertexBuffer{};
  SDL_GPUBuffer* indexBuffer{};
  // todo texture
  bool is3d{};
};

}  // namespace luanaut

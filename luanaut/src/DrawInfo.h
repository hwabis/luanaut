#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "Material.h"
#include "Mesh.h"

namespace luanaut {

struct DrawInfo {
  glm::mat4 worldTransform{1.0F};
  // what does 2D need or not need?
  Mesh* mesh = nullptr;
  Material* material = nullptr;
};

}  // namespace luanaut

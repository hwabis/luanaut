#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "Material.h"
#include "Mesh.h"

namespace lneng {

struct DrawInfo {
  glm::mat4 worldTransform{1.0F};
  // what does 2D need or not need?
  const Mesh* mesh = nullptr;
  const Material* material = nullptr;
};

}  // namespace lneng

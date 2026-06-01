#pragma once
#include <string>
#include <vector>
#include "Vertex.h"

namespace lneng {

struct ModelCreateInfo {
  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
  };

  std::vector<Mesh> meshes;
  // todo
  // textures
  // samplers

  // Set unique name if caching desired
  std::string name;
};

}  // namespace lneng

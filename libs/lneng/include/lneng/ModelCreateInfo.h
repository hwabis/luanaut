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

  struct Texture {
    std::vector<uint8_t> pixelsRgba8;
    uint32_t width{};
    uint32_t height{};
  };

  std::vector<Mesh> meshes;
  std::vector<Texture> textures;
  // todo add samplers to be able to use a non-default

  // Set unique name if caching desired
  std::string name;
};

}  // namespace lneng

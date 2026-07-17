#pragma once
#include <numeric>
#include <string>
#include <vector>
#include "Texture.h"
#include "Vertex.h"

namespace lneng {

struct ModelCreateInfo {
  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t textureIndex;
  };

  std::vector<Mesh> meshes;
  std::vector<Texture> textures;
  // todo add samplers to be able to use a non-default

  // Set unique name if caching desired
  std::string name;

  static auto Deindex(const Mesh& mesh) -> Mesh {
    Mesh flat;

    flat.vertices.reserve(mesh.indices.size());
    for (uint32_t idx : mesh.indices) {
      flat.vertices.push_back(mesh.vertices[idx]);
    }

    flat.indices.resize(flat.vertices.size());
    std::iota(flat.indices.begin(), flat.indices.end(), 0);

    return flat;
  }
};

}  // namespace lneng

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
    std::vector<Vertex> vertices;
    vertices.reserve(mesh.indices.size());
    for (uint32_t idx : mesh.indices) {
      vertices.push_back(mesh.vertices[idx]);
    }

    std::vector<uint32_t> indices;
    indices.resize(vertices.size());
    std::iota(indices.begin(), indices.end(), 0);

    return {
        .vertices = std::move(vertices),
        .indices = std::move(indices),
        .textureIndex = mesh.textureIndex,
    };
  }
};

}  // namespace lneng

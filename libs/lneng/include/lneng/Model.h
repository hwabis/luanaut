#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include "lneng/SdlHandles.h"

namespace lneng {

struct Model {
  struct Creation {
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

  // todo support multiple mesh like in creation
  SdlGpuBufferHandle vertexBuffer;
  SdlGpuBufferHandle indexBuffer;
  uint32_t indexBufferCount{};

  // todo add texture/sample structs
};

}  // namespace lneng

#pragma once
#include <string>
#include <vector>
#include "Vertex.h"

namespace lneng {

struct MeshInfo {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  std::string cacheKey;
};

}  // namespace lneng

#pragma once
#include <vector>
#include "MeshInfo.h"

namespace lneng {

struct ModelInfo {
  std::vector<MeshInfo> meshes;
  // todo
  // texture
  // sampler
};

}  // namespace lneng

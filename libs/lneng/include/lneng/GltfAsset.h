#pragma once
#include <vector>
#include "MeshInfo.h"

namespace lneng {

struct GltfAsset {
  std::vector<MeshInfo> meshes;
  // todo add material stuff ???
  // this is NOT MaterialInfo.h right?
};

}  // namespace lneng

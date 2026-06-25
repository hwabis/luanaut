#pragma once
#include <string>
#include "Texture.h"

namespace lneng {

struct SkyboxCreateInfo {
  Texture textureXPos;
  Texture textureXNeg;
  Texture textureYPos;
  Texture textureYNeg;
  Texture textureZPos;
  Texture textureZNeg;

  // Set unique name if caching desired
  std::string name;
};

}  // namespace lneng

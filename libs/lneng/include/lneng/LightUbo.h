#pragma once
#include "LightInfo.h"

namespace lneng {

constexpr int maxLights = 8;

struct LightUbo {
  // NOLINTNEXTLINE(modernize-avoid-c-arrays)
  LightInfo lights[maxLights]{};
  int lightCount{};
};

}  // namespace lneng

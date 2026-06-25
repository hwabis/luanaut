#pragma once
#include <vector>

namespace lneng {

struct Texture {
  std::vector<uint8_t> pixelsRgba8;
  uint32_t width{};
  uint32_t height{};
};

}  // namespace lneng

#pragma once
#include <glm/glm.hpp>

namespace lneng {

constexpr int fragShaderAlign = 16;
struct alignas(fragShaderAlign) LightInfo {
  glm::vec3 direction{};
  glm::vec3 color{};
};

}  // namespace lneng

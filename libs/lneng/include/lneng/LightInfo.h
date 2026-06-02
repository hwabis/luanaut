#pragma once
#include <glm/glm.hpp>

namespace lneng {

struct LightInfo {
  glm::vec3 direction{};
  float _pad0{};
  glm::vec3 color{};
  float _pad1{};
};

}  // namespace lneng

#pragma once
#include <glm/glm.hpp>

namespace lneng {

struct ParticleUbo {
  glm::mat4x4 viewProj;
  glm::vec3 cameraRight;
  float _pad0 = 0.0F;
  glm::vec3 cameraUp;
  float _pad1 = 0.0F;
  glm::vec3 particleWorldPos;
  float _pad2 = 0.0F;
  glm::vec3 particleColor;
  float particleSize;
  float alpha;
  float _pad3 = 0.0F;
  float _pad4 = 0.0F;
  float _pad5 = 0.0F;
};

}  // namespace lneng

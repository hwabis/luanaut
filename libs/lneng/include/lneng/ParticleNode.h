#pragma once
#include "Node.h"

namespace lneng {

class ParticleNode : public Node {
 public:
  ParticleNode(glm::vec3 color, float size, float alpha);

 protected:
  auto Load() -> void override;
  auto Draw(SceneInfo& out) -> void override;

 private:
  SdlGpuGraphicsPipelineHandle* pipeline_ = nullptr;
  glm::vec3 color_;
  float size_;
  float alpha_;
};

}  // namespace lneng

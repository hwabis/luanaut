#pragma once
#include "LightInfo.h"
#include "Node.h"

namespace lneng {

class LightNode : public Node {
 public:
  LightNode(LightInfo lightInfo);

 protected:
  auto Draw(SceneInfo& out) -> void override;

 private:
  LightInfo lightInfo_;
};

}  // namespace lneng

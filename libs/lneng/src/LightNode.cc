#include "lneng/LightNode.h"

namespace lneng {

LightNode::LightNode(LightInfo lightInfo) : lightInfo_(lightInfo) {}

auto LightNode::Draw(SceneInfo& out) -> void {
  out.lights.push_back(lightInfo_);
}

}  // namespace lneng

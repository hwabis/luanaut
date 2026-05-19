#pragma once
#include "Node.h"

namespace luanaut {

class MeshNode : public Node {
 protected:
  auto Load() -> void override;
  auto Draw(std::vector<DrawInfo>& out) -> void override;

 private:
  const Mesh* mesh_ = nullptr;
  const Material* material_ = nullptr;
};

}  // namespace luanaut

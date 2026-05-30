#pragma once
#include "GltfAsset.h"
#include "Node.h"

namespace lneng {

class MeshNode : public Node {
 public:
  MeshNode(GltfAsset asset);

 protected:
  auto Load() -> void override;
  auto Draw(std::vector<DrawInfo>& out) -> void override;

 private:
  GltfAsset asset_;

  const Mesh* mesh_ = nullptr;
  const Material* material_ = nullptr;
};

}  // namespace lneng

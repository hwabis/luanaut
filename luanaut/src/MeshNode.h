#pragma once
#include "Mesh.h"
#include "Node.h"

namespace luanaut {

class MeshNode : public Node {
 public:
  MeshNode(Mesh* mesh, Material* material) : mesh_(mesh), material_(material) {}

  // todo override draw, use the material

 private:
  Mesh* mesh_;
  Material* material_;
};

}  // namespace luanaut

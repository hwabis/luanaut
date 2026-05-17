#pragma once
#include "Mesh.h"
#include "Node.h"

namespace luanaut {

class MeshNode : public Node {
 public:
  MeshNode(Mesh mesh, SDL_GPUGraphicsPipeline* pipeline)
      : mesh_(std::move(mesh)), pipeline_(pipeline) {}

 private:
  Mesh mesh_;
  SDL_GPUGraphicsPipeline* pipeline_;
};

}  // namespace luanaut

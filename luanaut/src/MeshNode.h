#pragma once
#include "Mesh.h"
#include "Node.h"
#include "SdlHandles.h"

namespace luanaut {

class MeshNode : public Node {
 public:
  MeshNode(Mesh mesh, SdlGpuGraphicsPipelineHandle pipeline)
      : mesh_(std::move(mesh)), pipeline_(std::move(pipeline)) {}

 private:
  Mesh mesh_;
  SdlGpuGraphicsPipelineHandle pipeline_;
};

}  // namespace luanaut

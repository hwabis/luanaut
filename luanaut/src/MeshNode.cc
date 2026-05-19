#include "MeshNode.h"
#include "GpuResourceManager.h"

namespace luanaut {

auto MeshNode::Load() -> void {
  auto resources = deps_->Resolve<GpuResourceManager>();
  // todo right now this is just default stuff
  mesh_ = resources->CreateMesh();
  material_ = resources->CreateMaterial({
      .vertShaderPath = std::string(SHADER_BIN_DIR) + "/triangle.vert.spv",
      .fragShaderPath = std::string(SHADER_BIN_DIR) + "/triangle.frag.spv",
  });
}

auto MeshNode::Draw(std::vector<DrawInfo>& out) -> void {
  out.push_back(DrawInfo{
      .mesh = nullptr, // HIGH PRIORITY TODO USE AN ACTUAL MESH
      .material = material_,
  });
}

}  // namespace luanaut

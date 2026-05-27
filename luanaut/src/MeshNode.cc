#include "MeshNode.h"
#include "GpuResourceManager.h"
#include "Vertex.h"

namespace luanaut {

auto MeshNode::Load() -> void {
  auto* resources = deps_->Resolve<GpuResourceManager>();
  // todo load mesh from file (pass path thru ctor or smth)
  std::vector<Vertex> vertices = {{
                                      .pos = {0, 0.5, 0},
                                      .color = {1, 0, 0},
                                  },
                                  {
                                      .pos = {-0.5, -0.5, 0},
                                      .color = {0, 1, 0},
                                  },
                                  {
                                      .pos = {0.5, -0.5, 0},
                                      .color = {0, 0, 1},
                                  }};
  std::vector<uint32_t> indices = {0, 1, 2};
  mesh_ = resources->CreateMesh(vertices, indices);

  // todo same here
  material_ = resources->CreateMaterial({
      .vertShaderPath = std::string(SHADER_BIN_DIR) + "/triangle.vert.spv",
      .fragShaderPath = std::string(SHADER_BIN_DIR) + "/triangle.frag.spv",
  });
}

auto MeshNode::Draw(std::vector<DrawInfo>& out) -> void {
  out.push_back(DrawInfo{
      .mesh = mesh_,
      .material = material_,
  });
}

}  // namespace luanaut

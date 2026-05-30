#include "lneng/MeshNode.h"
#include "lneng/GpuResourceLoader.h"

namespace lneng {

MeshNode::MeshNode(GltfAsset asset) : asset_(std::move(asset)) {}

auto MeshNode::Load() -> void {
  // todo handle if asset has multiple meshes/materials
  // (vector<Mesh*> or something)

  auto* resources = deps_->Resolve<GpuResourceLoader>();

  mesh_ = resources->CreateMesh(asset_.meshes[0]);
  material_ = resources->CreateMaterial({
      .vertShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.vert.spv",
      .fragShaderPath = std::filesystem::path(LNENG_ASSETS_BIN_DIR) /
                        "shaders" / "default.frag.spv",
      .cacheKey = "default",
  });
}

auto MeshNode::Draw(std::vector<DrawInfo>& out) -> void {
  out.push_back(DrawInfo{
      .mesh = mesh_,
      .material = material_,
  });
}

}  // namespace lneng

#include "lneng/MeshNode.h"
#include "lneng/GpuResourceManager.h"
#include "lneng/Vertex.h"

namespace lneng {

auto MeshNode::Load() -> void {
  auto* resources = deps_->Resolve<GpuResourceManager>();
  // todo load mesh from file (pass path thru ctor or smth)
  std::vector<Vertex> vertices = {
      // Front (0, 0, 1)
      {
          .pos = {-0.5F, -0.5F, 0.5F},
          .normal = {0.0F, 0.0F, 1.0F},
          .uv = {0.0F, 0.0F},
          .color = {1, 0, 0},
      },
      {
          .pos = {0.5F, -0.5F, 0.5F},
          .normal = {0.0F, 0.0F, 1.0F},
          .uv = {1.0F, 0.0F},
          .color = {0, 1, 0},
      },
      {
          .pos = {0.5F, 0.5F, 0.5F},
          .normal = {0.0F, 0.0F, 1.0F},
          .uv = {1.0F, 1.0F},
          .color = {0, 0, 1},
      },
      {
          .pos = {-0.5F, 0.5F, 0.5F},
          .normal = {0.0F, 0.0F, 1.0F},
          .uv = {0.0F, 1.0F},
          .color = {1, 1, 1},
      },
      // Back (0, 0, -1)
      {
          .pos = {0.5F, -0.5F, -0.5F},
          .normal = {0.0F, 0.0F, -1.0F},
          .uv = {0.0F, 0.0F},
          .color = {1, 0, 0},
      },
      {
          .pos = {-0.5F, -0.5F, -0.5F},
          .normal = {0.0F, 0.0F, -1.0F},
          .uv = {1.0F, 0.0F},
          .color = {0, 1, 0},
      },
      {
          .pos = {-0.5F, 0.5F, -0.5F},
          .normal = {0.0F, 0.0F, -1.0F},
          .uv = {1.0F, 1.0F},
          .color = {0, 0, 1},
      },
      {
          .pos = {0.5F, 0.5F, -0.5F},
          .normal = {0.0F, 0.0F, -1.0F},
          .uv = {0.0F, 1.0F},
          .color = {1, 1, 1},
      },
      // Left (-1, 0, 0)
      {
          .pos = {-0.5F, -0.5F, -0.5F},
          .normal = {-1.0F, 0.0F, 0.0F},
          .uv = {0.0F, 0.0F},
          .color = {1, 0, 0},
      },
      {
          .pos = {-0.5F, -0.5F, 0.5F},
          .normal = {-1.0F, 0.0F, 0.0F},
          .uv = {1.0F, 0.0F},
          .color = {0, 1, 0},
      },
      {
          .pos = {-0.5F, 0.5F, 0.5F},
          .normal = {-1.0F, 0.0F, 0.0F},
          .uv = {1.0F, 1.0F},
          .color = {0, 0, 1},
      },
      {
          .pos = {-0.5F, 0.5F, -0.5F},
          .normal = {-1.0F, 0.0F, 0.0F},
          .uv = {0.0F, 1.0F},
          .color = {1, 1, 1},
      },
      // Right (1, 0, 0)
      {
          .pos = {0.5F, -0.5F, 0.5F},
          .normal = {1.0F, 0.0F, 0.0F},
          .uv = {0.0F, 0.0F},
          .color = {1, 0, 0},
      },
      {
          .pos = {0.5F, -0.5F, -0.5F},
          .normal = {1.0F, 0.0F, 0.0F},
          .uv = {1.0F, 0.0F},
          .color = {0, 1, 0},
      },
      {
          .pos = {0.5F, 0.5F, -0.5F},
          .normal = {1.0F, 0.0F, 0.0F},
          .uv = {1.0F, 1.0F},
          .color = {0, 0, 1},
      },
      {
          .pos = {0.5F, 0.5F, 0.5F},
          .normal = {1.0F, 0.0F, 0.0F},
          .uv = {0.0F, 1.0F},
          .color = {1, 1, 1},
      },
      // Top (0, 1, 0)
      {
          .pos = {-0.5F, 0.5F, -0.5F},
          .normal = {0.0F, 1.0F, 0.0F},
          .uv = {0.0F, 0.0F},
          .color = {1, 0, 0},
      },
      {
          .pos = {-0.5F, 0.5F, 0.5F},
          .normal = {0.0F, 1.0F, 0.0F},
          .uv = {1.0F, 0.0F},
          .color = {0, 1, 0},
      },
      {
          .pos = {0.5F, 0.5F, 0.5F},
          .normal = {0.0F, 1.0F, 0.0F},
          .uv = {1.0F, 1.0F},
          .color = {0, 0, 1},
      },
      {
          .pos = {0.5F, 0.5F, -0.5F},
          .normal = {0.0F, 1.0F, 0.0F},
          .uv = {0.0F, 1.0F},
          .color = {1, 1, 1},
      },
      // Bottom (0, -1, 0)
      {
          .pos = {-0.5F, -0.5F, 0.5F},
          .normal = {0.0F, -1.0F, 0.0F},
          .uv = {0.0F, 0.0F},
          .color = {1, 0, 0},
      },
      {
          .pos = {-0.5F, -0.5F, -0.5F},
          .normal = {0.0F, -1.0F, 0.0F},
          .uv = {1.0F, 0.0F},
          .color = {0, 1, 0},
      },
      {
          .pos = {0.5F, -0.5F, -0.5F},
          .normal = {0.0F, -1.0F, 0.0F},
          .uv = {1.0F, 1.0F},
          .color = {0, 0, 1},
      },
      {
          .pos = {0.5F, -0.5F, 0.5F},
          .normal = {0.0F, -1.0F, 0.0F},
          .uv = {0.0F, 1.0F},
          .color = {1, 1, 1},
      },
  };
  std::vector<uint32_t> indices = {
      0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
      12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
  };
  mesh_ = resources->CreateMesh({
      .vertices = std::move(vertices),
      .indices = std::move(indices),
      .cacheKey = "lolol",
  });

  // todo same here
  material_ = resources->CreateMaterial({
      .vertShaderPath = std::string(SHADER_BIN_DIR) + "/triangle.vert.spv",
      .fragShaderPath = std::string(SHADER_BIN_DIR) + "/triangle.frag.spv",
      .cacheKey = "lolol",
  });
}

auto MeshNode::Draw(std::vector<DrawInfo>& out) -> void {
  out.push_back(DrawInfo{
      .mesh = mesh_,
      .material = material_,
  });
}

}  // namespace lneng

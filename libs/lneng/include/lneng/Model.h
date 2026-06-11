#pragma once
#include <vector>
#include "lneng/SdlHandles.h"

namespace lneng {

struct Model {
  SdlGpuBufferHandle vertexBuffer;
  SdlGpuBufferHandle indexBuffer;
  uint32_t indexBufferCount{};
  std::vector<SdlGpuTextureHandle> textures;
  std::vector<SdlGpuSamplerHandle> samplers;

  // todo multiple meshes... for now we assume vertex/index buffers hold 1 mesh
  // struct Mesh {
  //   uint32_t firstIndex;
  //   uint32_t indexCount;
  //   uint32_t materialIndex;
  // };
  // std::vector<Mesh> meshes;
  // std::vector<Material> materials;
  // Material contains indices to the textures/samplers vectors or smth
  // Make sure to update createinfo if anything here changes...
};

}  // namespace lneng

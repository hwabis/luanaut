#pragma once
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include <vector>

namespace lneng {

struct SkyboxVertex {
  glm::vec3 pos{};

  static auto GetAttributes() -> std::vector<SDL_GPUVertexAttribute> {
    return {
        SDL_GPUVertexAttribute{
            .location = 0,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = offsetof(SkyboxVertex, pos),
        },
    };
  }
};

}  // namespace lneng

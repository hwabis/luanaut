#pragma once
#include <SDL3/SDL_gpu.h>
#include <array>
#include <glm/glm.hpp>

namespace lneng {

// "Fat" vertex. Shader doesn't need to use every one
struct Vertex {
  glm::vec3 pos{};
  glm::vec3 normal{};
  glm::vec2 uv{};
  glm::vec3 color{};

  static auto GetAttributes() -> std::array<SDL_GPUVertexAttribute, 4> {
    return {
        SDL_GPUVertexAttribute{
            .location = 0,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = offsetof(Vertex, pos),
        },
        SDL_GPUVertexAttribute{
            .location = 1,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = offsetof(Vertex, normal),
        },
        SDL_GPUVertexAttribute{
            .location = 2,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
            .offset = offsetof(Vertex, uv),
        },
        SDL_GPUVertexAttribute{
            .location = 3,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = offsetof(Vertex, color),
        },
    };
  }
};

}  // namespace lneng

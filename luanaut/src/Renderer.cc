#include "Renderer.h"
#include <stdexcept>

namespace luanaut {

Renderer::Renderer()
    : window_(SDL_CreateWindow("Luanaut", 0, 0, SDL_WINDOW_FULLSCREEN)),
      device_(SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr)) {
  if (window_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  if (device_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_ClaimWindowForGPUDevice(device_, window_);
}

auto Renderer::Draw(const std::vector<DrawInfo>& draws) -> void {
  SDL_GPUCommandBuffer* cmdBuf = SDL_AcquireGPUCommandBuffer(device_);
  if (cmdBuf == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_GPUTexture* swapchainTexture;
  Uint32 width = 0;
  Uint32 height = 0;
  if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuf, window_, &swapchainTexture,
                                             &width, &height)) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_GPUColorTargetInfo colorTarget{};
  colorTarget.texture = swapchainTexture;
  colorTarget.clear_color = {.r = 0, .g = 0, .b = 0, .a = 1},
  colorTarget.load_op = SDL_GPU_LOADOP_CLEAR;
  colorTarget.store_op = SDL_GPU_STOREOP_STORE;

  SDL_GPURenderPass* pass =
      SDL_BeginGPURenderPass(cmdBuf, &colorTarget, 1, nullptr);

  SDL_GPUGraphicsPipeline* boundPipeline = nullptr;

  for (const auto& info : draws) {
    if (info.material->pipeline != boundPipeline) {
      SDL_BindGPUGraphicsPipeline(pass, info.material->pipeline);
      boundPipeline = info.material->pipeline;
    }

    SDL_GPUViewport viewport{
        .x = 0,
        .y = 0,
        .w = static_cast<float>(width),
        .h = static_cast<float>(height),
        .min_depth = 0,
        .max_depth = 1,
    };
    SDL_SetGPUViewport(pass, &viewport);

    SDL_GPUBufferBinding vboBind{.buffer = info.mesh->vertexBuffer,
                                 .offset = 0};
    SDL_BindGPUVertexBuffers(pass, 0, &vboBind, 1);
    SDL_GPUBufferBinding iboBind{.buffer = info.mesh->indexBuffer, .offset = 0};
    SDL_BindGPUIndexBuffer(pass, &iboBind, SDL_GPU_INDEXELEMENTSIZE_32BIT);
    SDL_DrawGPUIndexedPrimitives(pass, info.mesh->indexBufferCount, 1, 0, 0, 0);
  }

  SDL_EndGPURenderPass(pass);
  SDL_SubmitGPUCommandBuffer(cmdBuf);
}

auto Renderer::GetDevice() const -> SDL_GPUDevice* {
  return device_;
}

auto Renderer::GetWindow() const -> SDL_Window* {
  return window_;
}

}  // namespace luanaut

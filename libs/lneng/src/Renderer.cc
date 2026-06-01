#include "lneng/Renderer.h"
#include <SDL3/SDL_gpu.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include "lneng/SdlHandles.h"

namespace lneng {

// todo ?? windowed for now to test resizing
constexpr int initialWidth = 800;
constexpr int initialHeight = 600;

Renderer::Renderer()
    : window_(SDL_CreateWindow("lneng",
                               initialWidth,
                               initialHeight,
                               SDL_WINDOW_RESIZABLE)),
      device_(SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr)) {
  if (window_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  if (device_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_ClaimWindowForGPUDevice(device_, window_);
}

auto Renderer::Draw(const SceneInfo& scene) -> void {
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

  bool windowSizeChanged = width != depthWidth_ || height != depthHeight_;

  if (windowSizeChanged) {
    SDL_GPUTextureCreateInfo textureCreate{
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
        .usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
        .width = width,
        .height = height,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
        .props = 0,
    };

    depthTexture_ = {device_, SDL_CreateGPUTexture(device_, &textureCreate)};
    depthWidth_ = width;
    depthHeight_ = height;
  }

  SDL_GPUDepthStencilTargetInfo depthInfo{};
  depthInfo.texture = depthTexture_;
  depthInfo.clear_depth = 1;
  depthInfo.load_op = SDL_GPU_LOADOP_CLEAR;
  depthInfo.store_op = SDL_GPU_STOREOP_DONT_CARE;
  depthInfo.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
  depthInfo.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
  depthInfo.cycle = !windowSizeChanged;

  SDL_GPURenderPass* pass =
      SDL_BeginGPURenderPass(cmdBuf, &colorTarget, 1, &depthInfo);

  SDL_GPUViewport viewport{
      .x = 0,
      .y = 0,
      .w = static_cast<float>(width),
      .h = static_cast<float>(height),
      .min_depth = 0,
      .max_depth = 1,
  };
  SDL_SetGPUViewport(pass, &viewport);

  // todo set up frag shader ubo with scene.lights

  SdlGpuGraphicsPipelineHandle* boundPipeline = nullptr;
  for (const auto& draw : scene.draws) {
    if (draw.pipeline != boundPipeline) {
      SDL_BindGPUGraphicsPipeline(pass, *draw.pipeline);
      boundPipeline = draw.pipeline;
    }

    // todo camera node
    glm::mat4 view = glm::lookAtLH(glm::vec3(0, 100, 300), glm::vec3(0, 0, 0),
                                   glm::vec3(0, 1, 0));
    constexpr float fov = 60;
    constexpr float zNear = 0.1F;
    constexpr float zFar = 1000;
    glm::mat4 proj = glm::perspectiveLH_ZO(
        glm::radians(fov),
        static_cast<float>(width) / static_cast<float>(height), zNear, zFar);
    glm::mat4 mvp = proj * view * draw.worldTransform;
    // todo feels like uploading smth random, is there a way to know without
    // manually checking the shader
    SDL_PushGPUVertexUniformData(cmdBuf, 0, &mvp, sizeof(mvp));

    SDL_GPUBufferBinding vboBind{.buffer = draw.model->vertexBuffer,
                                 .offset = 0};
    SDL_BindGPUVertexBuffers(pass, 0, &vboBind, 1);
    SDL_GPUBufferBinding iboBind{.buffer = draw.model->indexBuffer,
                                 .offset = 0};
    SDL_BindGPUIndexBuffer(pass, &iboBind, SDL_GPU_INDEXELEMENTSIZE_32BIT);

    SDL_DrawGPUIndexedPrimitives(pass, draw.model->indexBufferCount, 1, 0, 0,
                                 0);
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

}  // namespace lneng

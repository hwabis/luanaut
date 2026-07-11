#include "lneng/Renderer.h"
#include <SDL3/SDL_gpu.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include "lneng/LightUbo.h"
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

  const glm::mat4 proj = glm::perspectiveLH_ZO(
      glm::radians(scene.camera.fovDeg),
      static_cast<float>(width) / static_cast<float>(height),
      scene.camera.zNear, scene.camera.zFar);

  if (scene.skybox.has_value()) {
    glm::mat4 cameraRot = glm::mat4(glm::mat3(scene.camera.viewMat));
    drawSkybox(*scene.skybox, pass, cmdBuf, proj, cameraRot);
  }

  LightUbo lightUbo;
  for (const auto& light : scene.lights) {
    lightUbo.lights[lightUbo.lightCount] = light;
    ++lightUbo.lightCount;
  }
  SDL_PushGPUFragmentUniformData(cmdBuf, 0, &lightUbo, sizeof(lightUbo));

  SdlGpuGraphicsPipelineHandle* boundPipeline = nullptr;
  for (const auto& draw : scene.draws) {
    if (draw.pipeline != boundPipeline) {
      SDL_BindGPUGraphicsPipeline(pass, *draw.pipeline);
      boundPipeline = draw.pipeline;
    }

    glm::mat4 mvp = proj * scene.camera.viewMat * draw.worldTransform;
    SDL_PushGPUVertexUniformData(cmdBuf, 0, &mvp, sizeof(mvp));
    glm::mat4 normalMatrix{
        glm::transpose(glm::inverse(glm::mat3(draw.worldTransform)))};
    SDL_PushGPUVertexUniformData(cmdBuf, 1, &normalMatrix,
                                 sizeof(normalMatrix));

    SDL_GPUBufferBinding vboBind{.buffer = draw.model->vertexBuffer,
                                 .offset = 0};
    SDL_BindGPUVertexBuffers(pass, 0, &vboBind, 1);
    SDL_GPUBufferBinding iboBind{.buffer = draw.model->indexBuffer,
                                 .offset = 0};
    SDL_BindGPUIndexBuffer(pass, &iboBind, SDL_GPU_INDEXELEMENTSIZE_32BIT);

    SDL_PushGPUFragmentUniformData(cmdBuf, 1, &draw.material,
                                   sizeof(draw.material));

    for (const auto& submesh : draw.model->submeshes) {
      SDL_GPUTextureSamplerBinding bind{
          .texture = draw.model->textures[submesh.textureIndex],
          .sampler = draw.model->samplers[0],
      };
      SDL_BindGPUFragmentSamplers(pass, 0, &bind, 1);
      SDL_DrawGPUIndexedPrimitives(pass, submesh.indexCount, 1,
                                   submesh.firstIndex, 0, 0);
    }
  }

  for (const auto& fullscreen : scene.fullscreens) {
    SDL_BindGPUGraphicsPipeline(pass, *fullscreen.pipeline);
    SDL_PushGPUFragmentUniformData(cmdBuf, 0, &fullscreen.fadeColor,
                                   sizeof(fullscreen.fadeColor));
    SDL_DrawGPUPrimitives(pass, 3, 1, 0, 0);
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

auto Renderer::drawSkybox(SkyboxInfo skybox,
                          SDL_GPURenderPass* pass,
                          SDL_GPUCommandBuffer* cmdBuf,
                          glm::mat4 proj,
                          glm::mat4 cameraRot) -> void {
  SDL_BindGPUGraphicsPipeline(pass, *skybox.pipeline);

  glm::mat4 viewProj = proj * cameraRot;
  SDL_PushGPUVertexUniformData(cmdBuf, 0, &viewProj, sizeof(viewProj));

  SDL_GPUTextureSamplerBinding texBind{
      .texture = skybox.skybox->texture,
      .sampler = skybox.skybox->sampler,
  };
  SDL_BindGPUFragmentSamplers(pass, 0, &texBind, 1);

  SDL_GPUBufferBinding vbo{.buffer = skybox.skybox->vertexBuffer, .offset = 0};
  SDL_BindGPUVertexBuffers(pass, 0, &vbo, 1);
  SDL_GPUBufferBinding ibo{.buffer = skybox.skybox->indexBuffer, .offset = 0};
  SDL_BindGPUIndexBuffer(pass, &ibo, SDL_GPU_INDEXELEMENTSIZE_32BIT);

  SDL_DrawGPUIndexedPrimitives(pass, skybox.skybox->indexBufferCount, 1, 0, 0,
                               0);
}

}  // namespace lneng

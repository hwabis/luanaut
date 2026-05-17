#include "Game.h"
#include <SDL3/SDL_gpu.h>
#include <algorithm>
#include <array>
#include <fstream>
#include <stdexcept>

namespace luanaut {

Game::Game(std::unique_ptr<Node> root)
    : root_(std::move(root)),
      window_(SDL_CreateWindow("Luanaut", 0, 0, SDL_WINDOW_FULLSCREEN)),
      device_(SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr)) {
  if (window_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  if (device_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_ClaimWindowForGPUDevice(device_, window_);

  pipelines_.emplace("lololol",
                     SdlGpuGraphicsPipelineHandle{device_, createPipeline()});
}

auto Game::HandleEvent(const SDL_Event& event) -> void {
  if (root_->HandleEventSubTree(event)) {
    return;
  }

  if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    isRunning_ = false;
  }
}

auto Game::Update() -> void {
  root_->UpdateSubTree();

  std::vector<DrawInfo> drawInfos;
  root_->DrawSubTree(drawInfos);

  std::ranges::sort(drawInfos,
                    [](const DrawInfo& infoA, const DrawInfo& infoB) {
                      return infoA.pipeline < infoB.pipeline;
                    });

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

  SDL_BindGPUGraphicsPipeline(pass, pipelines_.at("lololol"));
  SDL_DrawGPUPrimitives(pass, 3, 1, 0, 0);
  SDL_EndGPURenderPass(pass);
  SDL_SubmitGPUCommandBuffer(cmdBuf);

  // SDL_GPUGraphicsPipeline* boundPipeline = nullptr;

  // for (const auto& info : drawInfos) {
  //   if (info.pipeline != boundPipeline) {
  //     SDL_BindGPUGraphicsPipeline(pass, info.pipeline);
  //     boundPipeline = info.pipeline;
  //   }

  //   SDL_GPUViewport viewport{
  //       .x = 0,
  //       .y = 0,
  //       .w = static_cast<float>(width),
  //       .h = static_cast<float>(height),
  //       .min_depth = 0,
  //       .max_depth = 1,
  //   };
  //   SDL_SetGPUViewport(pass, &viewport);

  //   SDL_GPUBufferBinding vboBind{.buffer = info.mesh->vertexBuffer,
  //                                .offset = 0};
  //   SDL_BindGPUVertexBuffers(pass, 0, &vboBind, 1);
  //   SDL_GPUBufferBinding iboBind{.buffer = info.mesh->indexBuffer, .offset =
  //   0}; SDL_BindGPUIndexBuffer(pass, &iboBind,
  //   SDL_GPU_INDEXELEMENTSIZE_32BIT); SDL_DrawGPUIndexedPrimitives(pass,
  //   info.mesh->indexBufferCount, 1, 0, 0, 0);
  // }

  // SDL_EndGPURenderPass(pass);
  // SDL_SubmitGPUCommandBuffer(cmdBuf);
}

auto Game::SetScene(std::unique_ptr<Node> root) -> void {
  root_ = std::move(root);
}

auto Game::IsRunning() const -> bool {
  return isRunning_;
}

auto Game::createPipeline() -> SDL_GPUGraphicsPipeline* {
  auto vertCode = readFile(std::string(SHADER_BIN_DIR) + "/triangle.vert.spv");
  SDL_GPUShaderCreateInfo vertInfo = {
      .code_size = vertCode.size(),
      .code = vertCode.data(),
      .entrypoint = "vertMain",
      .format = SDL_GPU_SHADERFORMAT_SPIRV,
      .stage = SDL_GPU_SHADERSTAGE_VERTEX,
      .num_samplers = 0,
      .num_storage_textures = 0,
      .num_storage_buffers = 0,
      .num_uniform_buffers = 0,
      .props = 0,
  };
  SDL_GPUShader* vertShader = SDL_CreateGPUShader(device_, &vertInfo);
  if (vertShader == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  auto fragCode = readFile(std::string(SHADER_BIN_DIR) + "/triangle.frag.spv");
  SDL_GPUShaderCreateInfo fragInfo = {
      .code_size = fragCode.size(),
      .code = fragCode.data(),
      .entrypoint = "fragMain",
      .format = SDL_GPU_SHADERFORMAT_SPIRV,
      .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
      .num_samplers = 0,
      .num_storage_textures = 0,
      .num_storage_buffers = 0,
      .num_uniform_buffers = 0,
      .props = 0,
  };
  SDL_GPUShader* fragShader = SDL_CreateGPUShader(device_, &fragInfo);
  if (fragShader == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_GPUVertexBufferDescription vertDesc = {
      .slot = 0,
      .pitch = 0,
      .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
      .instance_step_rate = 0,
  };

  std::array<SDL_GPUVertexAttribute, 0> vertAttributes = {};

  SDL_GPUVertexInputState vertInputState = {
      .vertex_buffer_descriptions = &vertDesc,
      .num_vertex_buffers = 0,
      .vertex_attributes = vertAttributes.data(),
      .num_vertex_attributes = 0,
  };

  SDL_GPURasterizerState rasterizerState = {};
  rasterizerState.fill_mode = SDL_GPU_FILLMODE_FILL;
  rasterizerState.cull_mode = SDL_GPU_CULLMODE_BACK;
  rasterizerState.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;

  SDL_GPUMultisampleState multisampleState = {};
  multisampleState.sample_count = SDL_GPU_SAMPLECOUNT_1;

  SDL_GPUDepthStencilState depthStencilState = {};

  SDL_GPUColorTargetDescription colorTargetDesc{
      .format = SDL_GetGPUSwapchainTextureFormat(device_, window_),
      .blend_state = {},
  };

  SDL_GPUGraphicsPipelineTargetInfo targetInfo = {};
  targetInfo.color_target_descriptions = &colorTargetDesc;
  targetInfo.num_color_targets = 1;
  targetInfo.has_depth_stencil_target = false;

  SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{
      .vertex_shader = vertShader,
      .fragment_shader = fragShader,
      .vertex_input_state = vertInputState,
      .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
      .rasterizer_state = rasterizerState,
      .multisample_state = multisampleState,
      .depth_stencil_state = depthStencilState,
      .target_info = targetInfo,
      .props = 0,
  };

  auto* pipeline = SDL_CreateGPUGraphicsPipeline(device_, &pipelineInfo);
  if (pipeline == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_ReleaseGPUShader(device_, vertShader);
  SDL_ReleaseGPUShader(device_, fragShader);

  return pipeline;
}

auto Game::readFile(const std::string& fileName) -> std::vector<uint8_t> {
  std::ifstream file(fileName, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open file: " + fileName);
  }

  const size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<uint8_t> buffer(fileSize);
  file.seekg(0);
  file.read(reinterpret_cast<char*>(buffer.data()),
            static_cast<std::streamsize>(fileSize));

  return buffer;
}

}  // namespace luanaut

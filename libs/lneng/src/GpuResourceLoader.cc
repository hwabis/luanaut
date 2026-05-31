#include "lneng/GpuResourceLoader.h"
#include <array>
#include <fstream>
#include <stdexcept>
#include "lneng/SdlHandles.h"

namespace lneng {

GpuResourceLoader::GpuResourceLoader(SDL_Window* window, SDL_GPUDevice* device)
    : window_(window), device_(device) {}

auto GpuResourceLoader::CreateMesh(const MeshInfo& info) -> Mesh* {
  if (auto itr = meshes_.find(info.cacheKey); itr != meshes_.end()) {
    return itr->second.get();
  }

  SDL_GPUBufferCreateInfo vboInfo = {
      .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
      .size = static_cast<Uint32>(info.vertices.size() * sizeof(Vertex)),
      .props = 0,
  };
  SDL_GPUBuffer* vbo = SDL_CreateGPUBuffer(device_, &vboInfo);
  uploadToBuffer(vbo, info.vertices);

  SDL_GPUBufferCreateInfo iboInfo = {
      .usage = SDL_GPU_BUFFERUSAGE_INDEX,
      .size = static_cast<Uint32>(info.indices.size() * sizeof(uint32_t)),
      .props = 0,
  };
  SDL_GPUBuffer* ibo = SDL_CreateGPUBuffer(device_, &iboInfo);
  uploadToBuffer(ibo, info.indices);

  meshes_[info.cacheKey] = std::make_unique<Mesh>(Mesh{
      .vertexBuffer = {device_, vbo},
      .indexBuffer = {device_, ibo},
      .indexBufferCount = static_cast<Uint32>(info.indices.size()),
  });

  return meshes_[info.cacheKey].get();
}

auto GpuResourceLoader::CreateGpuGraphicsPipeline(
    const GpuGraphicsPipelineInfo& info) -> SdlGpuGraphicsPipelineHandle* {
  if (auto itr = pipelines_.find(info.GetHashKey()); itr != pipelines_.end()) {
    return itr->second.get();
  }

  auto vertCode = readFile(info.vertShaderPath);
  SDL_GPUShaderCreateInfo vertInfo = {
      .code_size = vertCode.size(),
      .code = vertCode.data(),
      .entrypoint = "vertMain",
      .format = SDL_GPU_SHADERFORMAT_SPIRV,
      .stage = SDL_GPU_SHADERSTAGE_VERTEX,
      .num_samplers = 0,
      .num_storage_textures = 0,
      .num_storage_buffers = 0,
      .num_uniform_buffers = 1,
      .props = 0,
  };
  SDL_GPUShader* vertShader = SDL_CreateGPUShader(device_, &vertInfo);
  if (vertShader == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  auto fragCode = readFile(info.fragShaderPath);
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
      .pitch = sizeof(Vertex),
      .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
      .instance_step_rate = 0,
  };

  auto vertAttributes = Vertex::GetAttributes();

  SDL_GPUVertexInputState vertInputState = {
      .vertex_buffer_descriptions = &vertDesc,
      .num_vertex_buffers = 1,
      .vertex_attributes = vertAttributes.data(),
      .num_vertex_attributes = vertAttributes.size(),
  };

  SDL_GPURasterizerState rasterizerState = {};
  rasterizerState.fill_mode = SDL_GPU_FILLMODE_FILL;
  rasterizerState.cull_mode = SDL_GPU_CULLMODE_BACK;
  rasterizerState.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;

  SDL_GPUMultisampleState multisampleState = {};
  multisampleState.sample_count = SDL_GPU_SAMPLECOUNT_1;

  SDL_GPUDepthStencilState depthStencilState = {};
  depthStencilState.compare_op = SDL_GPU_COMPAREOP_LESS;
  depthStencilState.enable_depth_test = true;
  depthStencilState.enable_depth_write = true;

  SDL_GPUColorTargetDescription colorTargetDesc{
      .format = SDL_GetGPUSwapchainTextureFormat(device_, window_),
      .blend_state = {},
  };

  SDL_GPUGraphicsPipelineTargetInfo targetInfo = {};
  targetInfo.color_target_descriptions = &colorTargetDesc;
  targetInfo.num_color_targets = 1;
  targetInfo.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
  targetInfo.has_depth_stencil_target = true;

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

  pipelines_[info.GetHashKey()] =
      std::make_unique<SdlGpuGraphicsPipelineHandle>(device_, pipeline);

  SDL_ReleaseGPUShader(device_, vertShader);
  SDL_ReleaseGPUShader(device_, fragShader);

  return pipelines_[info.GetHashKey()].get();
}

auto GpuResourceLoader::readFile(const std::filesystem::path& path)
    -> std::vector<uint8_t> {
  std::ifstream file(path, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open file: " + path.string());
  }

  const size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<uint8_t> buffer(fileSize);
  file.seekg(0);
  file.read(reinterpret_cast<char*>(buffer.data()),
            static_cast<std::streamsize>(fileSize));

  return buffer;
}

}  // namespace lneng

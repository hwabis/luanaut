#include "lneng/GpuResourceLoader.h"
#include <SDL3/SDL_gpu.h>
#include <fstream>
#include <stdexcept>
#include "lneng/SdlHandles.h"

namespace lneng {

GpuResourceLoader::GpuResourceLoader(SDL_Window* window, SDL_GPUDevice* device)
    : window_(window), device_(device) {}

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
      .num_uniform_buffers = 2,
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
      .num_samplers = 1,
      .num_storage_textures = 0,
      .num_storage_buffers = 0,
      .num_uniform_buffers = 1,
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

  SDL_GPUVertexInputState vertInputState = {
      .vertex_buffer_descriptions = &vertDesc,
      .num_vertex_buffers = 1,
      .vertex_attributes = info.vertShaderAttributes.data(),
      .num_vertex_attributes =
          static_cast<Uint32>(info.vertShaderAttributes.size()),
  };

  SDL_GPURasterizerState rasterizerState = {};
  rasterizerState.fill_mode = SDL_GPU_FILLMODE_FILL;
  rasterizerState.cull_mode = SDL_GPU_CULLMODE_BACK;
  rasterizerState.front_face = SDL_GPU_FRONTFACE_CLOCKWISE;

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

auto GpuResourceLoader::CreateModel(const ModelCreateInfo& info) -> Model* {
  if (!info.name.empty()) {
    if (auto itr = models_.find(info.name); itr != models_.end()) {
      return itr->second.get();
    }
  }

  if (info.meshes.size() != 1) {
    // todo remove
    throw std::runtime_error("CreateModel: only single-mesh models supported");
  }

  // todo handles multiple meshes instead of [0]... how will this work ???
  SDL_GPUBufferCreateInfo vboInfo = {
      .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
      .size =
          static_cast<Uint32>(info.meshes[0].vertices.size() * sizeof(Vertex)),
      .props = 0,
  };
  SDL_GPUBuffer* vbo = SDL_CreateGPUBuffer(device_, &vboInfo);
  uploadToBuffer(vbo, info.meshes[0].vertices);

  SDL_GPUBufferCreateInfo iboInfo = {
      .usage = SDL_GPU_BUFFERUSAGE_INDEX,
      .size =
          static_cast<Uint32>(info.meshes[0].indices.size() * sizeof(uint32_t)),
      .props = 0,
  };
  SDL_GPUBuffer* ibo = SDL_CreateGPUBuffer(device_, &iboInfo);
  uploadToBuffer(ibo, info.meshes[0].indices);

  std::vector<SdlGpuTextureHandle> assetTextures;
  for (const auto& texInfo : info.textures) {
    SDL_GPUTextureCreateInfo texCreateInfo = {
        .type = SDL_GPU_TEXTURETYPE_2D,
        // todo srgb
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = static_cast<Uint32>(texInfo.width),
        .height = static_cast<Uint32>(texInfo.height),
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
        .props = 0};

    SDL_GPUTexture* texture = SDL_CreateGPUTexture(device_, &texCreateInfo);
    uploadToTexture(texture, texInfo);
    assetTextures.emplace_back(device_, texture);
  }

  SDL_GPUSamplerCreateInfo samplerInfo{};
  samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
  samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
  samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
  samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
  samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
  samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
  samplerInfo.max_lod = 1000;
  SDL_GPUSampler* defaultSampler = SDL_CreateGPUSampler(device_, &samplerInfo);
  std::vector<SdlGpuSamplerHandle> samplers;
  samplers.emplace_back(device_, defaultSampler);

  auto model = std::make_unique<Model>(Model{
      .vertexBuffer = {device_, vbo},
      .indexBuffer = {device_, ibo},
      .indexBufferCount = static_cast<Uint32>(info.meshes[0].indices.size()),
      .textures = std::move(assetTextures),
      .samplers = std::move(samplers),
  });

  if (!info.name.empty()) {
    models_[info.name] = std::move(model);
    return models_[info.name].get();
  }

  uncachedModels_.push_back(std::move(model));
  return uncachedModels_.back().get();
}

auto GpuResourceLoader::CreateSkybox(SkyboxCreateInfo& info) -> Skybox* {
  if (!info.name.empty()) {
    if (auto itr = skyboxes_.find(info.name); itr != skyboxes_.end()) {
      return itr->second.get();
    }
  }

  if (info.textureXPos.width != info.textureXNeg.width ||
      info.textureXNeg.width != info.textureYPos.width ||
      info.textureYPos.width != info.textureYNeg.width ||
      info.textureYNeg.width != info.textureZPos.width ||
      info.textureZPos.width != info.textureZNeg.width) {
    throw std::runtime_error("All textures need to be the same dimensions");
  }

  constexpr int skyboxLayerCount = 6;
  SDL_GPUTextureCreateInfo texCreateInfo = {
      .type = SDL_GPU_TEXTURETYPE_CUBE,
      .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
      .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
      .width = static_cast<Uint32>(info.textureXPos.width),
      .height = static_cast<Uint32>(info.textureXPos.height),
      .layer_count_or_depth = skyboxLayerCount,
      .num_levels = 1,
      .sample_count = SDL_GPU_SAMPLECOUNT_1,
      .props = 0};

  SDL_GPUTexture* texture = SDL_CreateGPUTexture(device_, &texCreateInfo);
  uploadToTexture(texture, info.textureXPos, 0);
  uploadToTexture(texture, info.textureXNeg, 1);
  uploadToTexture(texture, info.textureYPos, 2);
  uploadToTexture(texture, info.textureYNeg, 3);
  uploadToTexture(texture, info.textureZPos, 4);
  uploadToTexture(texture, info.textureZNeg, 5);

  auto skybox = std::make_unique<Skybox>(Skybox{
      .texture = {device_, texture},
  });

  if (!info.name.empty()) {
    skyboxes_[info.name] = std::move(skybox);
    return skyboxes_[info.name].get();
  }

  uncachedSkyboxes_.push_back(std::move(skybox));
  return uncachedSkyboxes_.back().get();
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

auto GpuResourceLoader::uploadToTexture(SDL_GPUTexture* target,
                                        const Texture& texInfo,
                                        Uint32 layer) -> void {
  uint32_t bufferSize = texInfo.pixelsRgba8.size();

  SDL_GPUTransferBufferCreateInfo transferInfo = {
      .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
      .size = bufferSize,
      .props = 0};
  SDL_GPUTransferBuffer* transferBuffer =
      SDL_CreateGPUTransferBuffer(device_, &transferInfo);

  void* mappedMem = SDL_MapGPUTransferBuffer(device_, transferBuffer, false);
  std::memcpy(mappedMem, texInfo.pixelsRgba8.data(), bufferSize);
  SDL_UnmapGPUTransferBuffer(device_, transferBuffer);

  SDL_GPUCommandBuffer* cmdBuf = SDL_AcquireGPUCommandBuffer(device_);
  SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuf);

  SDL_GPUTextureTransferInfo sourceInfo = {.transfer_buffer = transferBuffer,
                                           .offset = 0,
                                           .pixels_per_row = texInfo.width,
                                           .rows_per_layer = texInfo.height};

  SDL_GPUTextureRegion destRegion = {.texture = target,
                                     .mip_level = 0,
                                     .layer = layer,
                                     .x = 0,
                                     .y = 0,
                                     .z = 0,
                                     .w = texInfo.width,
                                     .h = texInfo.height,
                                     .d = 1};

  SDL_UploadToGPUTexture(copyPass, &sourceInfo, &destRegion, false);

  SDL_EndGPUCopyPass(copyPass);
  SDL_SubmitGPUCommandBuffer(cmdBuf);
  SDL_ReleaseGPUTransferBuffer(device_, transferBuffer);
}

}  // namespace lneng

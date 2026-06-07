#pragma once
#include <SDL3/SDL.h>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include "GpuGraphicsPipelineInfo.h"
#include "Model.h"
#include "ModelCreateInfo.h"
#include "SdlHandles.h"

namespace lneng {

class GpuResourceLoader {
 public:
  GpuResourceLoader(SDL_Window* window, SDL_GPUDevice* device);

  auto CreateGpuGraphicsPipeline(const GpuGraphicsPipelineInfo& info)
      -> SdlGpuGraphicsPipelineHandle*;
  auto CreateModel(const ModelCreateInfo& info) -> Model*;

 private:
  static auto readFile(const std::filesystem::path& path)
      -> std::vector<uint8_t>;
  template <typename T>
  auto uploadToBuffer(SDL_GPUBuffer* target, const std::vector<T>& data)
      -> void {
    auto size = static_cast<Uint32>(data.size() * sizeof(T));

    SDL_GPUTransferBufferCreateInfo transferInfo = {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = size,
        .props = 0,
    };
    SDL_GPUTransferBuffer* transferBuf =
        SDL_CreateGPUTransferBuffer(device_, &transferInfo);

    void* mapped = SDL_MapGPUTransferBuffer(device_, transferBuf, false);
    memcpy(mapped, data.data(), size);
    SDL_UnmapGPUTransferBuffer(device_, transferBuf);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device_);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

    SDL_GPUTransferBufferLocation src = {.transfer_buffer = transferBuf,
                                         .offset = 0};
    SDL_GPUBufferRegion dst = {.buffer = target, .offset = 0, .size = size};
    SDL_UploadToGPUBuffer(copyPass, &src, &dst, false);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmd);
    SDL_ReleaseGPUTransferBuffer(device_, transferBuf);
  }
  auto uploadToTexture(SDL_GPUTexture* target,
                       const ModelCreateInfo::Texture& texInfo) -> void;

  std::unordered_map<std::string, std::unique_ptr<SdlGpuGraphicsPipelineHandle>>
      pipelines_;
  std::unordered_map<std::string, std::unique_ptr<Model>> models_;

  // todo this grows forever lol
  std::vector<std::unique_ptr<Model>> uncachedModels_;

  SDL_Window* window_;
  SDL_GPUDevice* device_;
};

}  // namespace lneng

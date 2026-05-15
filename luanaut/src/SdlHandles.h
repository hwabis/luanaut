#pragma once
#include <SDL3/SDL.h>

namespace luanaut {

class SdlWindowHandle {
 public:
  SdlWindowHandle() = default;

  SdlWindowHandle(SDL_Window* window) : window_(window) {}

  ~SdlWindowHandle() {
    if (window_ != nullptr) {
      SDL_DestroyWindow(window_);
    }
  }

  SdlWindowHandle(const SdlWindowHandle&) = delete;
  auto operator=(const SdlWindowHandle&) -> SdlWindowHandle& = delete;

  SdlWindowHandle(SdlWindowHandle&& other) noexcept : window_(other.window_) {
    other.window_ = nullptr;
  }

  auto operator=(SdlWindowHandle&& other) noexcept -> SdlWindowHandle& {
    if (this != &other) {
      if (window_ != nullptr) {
        SDL_DestroyWindow(window_);
      }

      window_ = other.window_;
      other.window_ = nullptr;
    }

    return *this;
  }

  operator SDL_Window*() const { return window_; }

 private:
  SDL_Window* window_ = nullptr;
};

class SdlGpuDeviceHandle {
 public:
  SdlGpuDeviceHandle() = default;

  SdlGpuDeviceHandle(SDL_GPUDevice* device) : device_(device) {}

  ~SdlGpuDeviceHandle() {
    if (device_ != nullptr) {
      SDL_DestroyGPUDevice(device_);
    }
  }

  SdlGpuDeviceHandle(const SdlGpuDeviceHandle&) = delete;
  auto operator=(const SdlGpuDeviceHandle&) -> SdlGpuDeviceHandle& = delete;

  SdlGpuDeviceHandle(SdlGpuDeviceHandle&& other) noexcept
      : device_(other.device_) {
    other.device_ = nullptr;
  }

  auto operator=(SdlGpuDeviceHandle&& other) noexcept -> SdlGpuDeviceHandle& {
    if (this != &other) {
      if (device_ != nullptr) {
        SDL_DestroyGPUDevice(device_);
      }

      device_ = other.device_;
      other.device_ = nullptr;
    }

    return *this;
  }

  operator SDL_GPUDevice*() const { return device_; }

 private:
  SDL_GPUDevice* device_ = nullptr;
};

class SdlGpuBufferHandle {
 public:
  SdlGpuBufferHandle() = default;

  SdlGpuBufferHandle(SDL_GPUDevice* device, SDL_GPUBuffer* buffer)
      : device_(device), buffer_(buffer) {}

  ~SdlGpuBufferHandle() {
    if (buffer_ != nullptr) {
      SDL_ReleaseGPUBuffer(device_, buffer_);
    }
  }

  SdlGpuBufferHandle(const SdlGpuBufferHandle&) = delete;
  auto operator=(const SdlGpuBufferHandle&) -> SdlGpuBufferHandle& = delete;

  SdlGpuBufferHandle(SdlGpuBufferHandle&& other) noexcept
      : device_(other.device_), buffer_(other.buffer_) {
    other.device_ = nullptr;
    other.buffer_ = nullptr;
  }

  auto operator=(SdlGpuBufferHandle&& other) noexcept -> SdlGpuBufferHandle& {
    if (this != &other) {
      if (buffer_ != nullptr) {
        SDL_ReleaseGPUBuffer(device_, buffer_);
      }

      device_ = other.device_;
      buffer_ = other.buffer_;

      other.device_ = nullptr;
      other.buffer_ = nullptr;
    }

    return *this;
  }

  operator SDL_GPUBuffer*() const { return buffer_; }

 private:
  SDL_GPUDevice* device_ = nullptr;
  SDL_GPUBuffer* buffer_ = nullptr;
};

class SdlGpuGraphicsPipelineHandle {
 public:
  SdlGpuGraphicsPipelineHandle() = default;

  SdlGpuGraphicsPipelineHandle(SDL_GPUDevice* device,
                               SDL_GPUGraphicsPipeline* pipeline)
      : device_(device), pipeline_(pipeline) {}

  ~SdlGpuGraphicsPipelineHandle() {
    if (pipeline_ != nullptr) {
      SDL_ReleaseGPUGraphicsPipeline(device_, pipeline_);
    }
  }

  SdlGpuGraphicsPipelineHandle(const SdlGpuGraphicsPipelineHandle&) = delete;

  auto operator=(const SdlGpuGraphicsPipelineHandle&)
      -> SdlGpuGraphicsPipelineHandle& = delete;

  SdlGpuGraphicsPipelineHandle(SdlGpuGraphicsPipelineHandle&& other) noexcept
      : device_(other.device_), pipeline_(other.pipeline_) {
    other.device_ = nullptr;
    other.pipeline_ = nullptr;
  }

  auto operator=(SdlGpuGraphicsPipelineHandle&& other) noexcept
      -> SdlGpuGraphicsPipelineHandle& {
    if (this != &other) {
      if (pipeline_ != nullptr) {
        SDL_ReleaseGPUGraphicsPipeline(device_, pipeline_);
      }

      device_ = other.device_;
      pipeline_ = other.pipeline_;

      other.device_ = nullptr;
      other.pipeline_ = nullptr;
    }

    return *this;
  }

  operator SDL_GPUGraphicsPipeline*() const { return pipeline_; }

 private:
  SDL_GPUDevice* device_ = nullptr;
  SDL_GPUGraphicsPipeline* pipeline_ = nullptr;
};

}  // namespace luanaut

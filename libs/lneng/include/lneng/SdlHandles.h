#pragma once
#include <SDL3/SDL.h>

namespace lneng {

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

template <typename T, auto ReleaseFunc>
class SdlGpuHandle {
 public:
  SdlGpuHandle() = default;
  SdlGpuHandle(SDL_GPUDevice* device, T* handle)
      : device_(device), handle_(handle) {}
  ~SdlGpuHandle() {
    if (handle_) {
      ReleaseFunc(device_, handle_);
    }
  }

  SdlGpuHandle(const SdlGpuHandle&) = delete;
  auto operator=(const SdlGpuHandle&) -> SdlGpuHandle& = delete;
  SdlGpuHandle(SdlGpuHandle&& other) noexcept
      : device_(other.device_), handle_(other.handle_) {
    other.device_ = nullptr;
    other.handle_ = nullptr;
  }
  auto operator=(SdlGpuHandle&& other) noexcept -> SdlGpuHandle& {
    if (this != &other) {
      if (handle_) {
        ReleaseFunc(device_, handle_);
      }
      device_ = other.device_;
      handle_ = other.handle_;
      other.device_ = nullptr;
      other.handle_ = nullptr;
    }
    return *this;
  }

  operator T*() const { return handle_; }

 private:
  SDL_GPUDevice* device_ = nullptr;
  T* handle_ = nullptr;
};

using SdlGpuBufferHandle = SdlGpuHandle<SDL_GPUBuffer, SDL_ReleaseGPUBuffer>;
using SdlGpuTextureHandle = SdlGpuHandle<SDL_GPUTexture, SDL_ReleaseGPUTexture>;
using SdlGpuGraphicsPipelineHandle =
    SdlGpuHandle<SDL_GPUGraphicsPipeline, SDL_ReleaseGPUGraphicsPipeline>;

}  // namespace lneng

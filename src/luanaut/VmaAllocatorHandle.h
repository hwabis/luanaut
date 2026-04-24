#pragma once
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_raii.hpp>

class VmaAllocatorHandle {
 public:
  VmaAllocatorHandle(VkPhysicalDevice physicalDevice,
                     VkDevice device,
                     VkInstance instance) {
    VmaAllocatorCreateInfo info{.flags = 0,
                                .physicalDevice = physicalDevice,
                                .device = device,
                                .preferredLargeHeapBlockSize = 0,
                                .pAllocationCallbacks = nullptr,
                                .pDeviceMemoryCallbacks = nullptr,
                                .pHeapSizeLimit = nullptr,
                                .pVulkanFunctions = nullptr,
                                .instance = instance,
                                .vulkanApiVersion = vk::ApiVersion13,
                                .pTypeExternalMemoryHandleTypes = nullptr};
    vmaCreateAllocator(&info, &allocator_);
  }
  ~VmaAllocatorHandle() { vmaDestroyAllocator(allocator_); }
  operator VmaAllocator() const { return allocator_; }

 private:
  VmaAllocator allocator_ = nullptr;
};

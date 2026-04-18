#pragma once
#include <SDL3/SDL_vulkan.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_raii.hpp>

namespace luanaut {

class VulkanStuff {
 public:
  VulkanStuff(SDL_Window* window);
  ~VulkanStuff();

  auto DrawFrame() -> void;
  auto NotifyResize() -> void;

 private:
  struct SwapchainBundle {
    vk::raii::SwapchainKHR swapchain;
    struct ImageInfo {
      vk::Image image;
      vk::raii::ImageView imageView;
      vk::raii::Semaphore renderFinishedSemaphore;
    };
    std::vector<ImageInfo> imagesInfo;
    vk::SurfaceFormatKHR format;
    vk::Extent2D extent;
  };

  struct CommandBufferInfo {
    vk::raii::Fence fence;
    vk::raii::Semaphore presentCompleteSemaphore;
  };

  auto recreateSwapchain() -> void;

  static auto recordCommandBuffer(const vk::raii::CommandBuffer& cmd,
                                  const SwapchainBundle::ImageInfo& imageInfo,
                                  const vk::Extent2D& extent,
                                  const vk::raii::Pipeline& pipeline,
                                  VkBuffer vertexBuffer) -> void;
  static auto transitionToColorAttachment(const vk::raii::CommandBuffer& cmd,
                                          vk::Image image) -> void;
  static auto transitionToPresent(const vk::raii::CommandBuffer& cmd,
                                  vk::Image image) -> void;

  static auto createInstance(const vk::raii::Context& context)
      -> vk::raii::Instance;
  static auto createDebugMessenger(const vk::raii::Instance& instance)
      -> vk::raii::DebugUtilsMessengerEXT;
  static VKAPI_ATTR vk::Bool32 VKAPI_CALL
  // NOLINTNEXTLINE(modernize-use-trailing-return-type)
  debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                vk::DebugUtilsMessageTypeFlagsEXT messageTypes,
                const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* /*pUserData*/);
  static auto createSurface(SDL_Window* window,
                            const vk::raii::Instance& instance)
      -> vk::raii::SurfaceKHR;
  static auto createPhysicalDevice(const vk::raii::Instance& instance)
      -> vk::raii::PhysicalDevice;
  static auto createLogicalDevice(
      const vk::raii::SurfaceKHR& surface,
      const vk::raii::PhysicalDevice& physicalDevice) -> vk::raii::Device;
  static auto findGraphicsQueueIndex(
      const vk::raii::SurfaceKHR& surface,
      const vk::raii::PhysicalDevice& physicalDevice) -> uint32_t;
  static auto createSwapchainBundle(
      SDL_Window* window,
      const vk::raii::SurfaceKHR& surface,
      const vk::raii::PhysicalDevice& physicalDevice,
      const vk::raii::Device& device,
      vk::SwapchainKHR oldSwapchain = VK_NULL_HANDLE) -> SwapchainBundle;
  static auto createGraphicsPipeline(const vk::raii::Device& device,
                                     const SwapchainBundle& swapchainBundle,
                                     const vk::raii::PipelineLayout& layout)
      -> vk::raii::Pipeline;
  static auto readFile(const std::string& filename) -> std::vector<uint32_t>;
  static auto createCommandPool(const vk::raii::SurfaceKHR& surface,
                                const vk::raii::Device& device,
                                const vk::raii::PhysicalDevice& physicalDevice)
      -> vk::raii::CommandPool;
  static auto createCommandBuffers(const vk::raii::Device& device,
                                   const vk::raii::CommandPool& commandPool)
      -> vk::raii::CommandBuffers;
  static auto createCommandBuffersInfo(const vk::raii::Device& device)
      -> std::vector<CommandBufferInfo>;

  SDL_Window* window_;

  vk::raii::Context context_;
  vk::raii::Instance instance_;
  vk::raii::DebugUtilsMessengerEXT debugMessenger_;

  vk::raii::SurfaceKHR surface_;
  vk::raii::PhysicalDevice physicalDevice_;
  vk::raii::Device device_;
  vk::raii::Queue graphicsQueue_;

  SwapchainBundle swapchainBundle_;

  vk::raii::PipelineLayout pipelineLayout_;
  vk::raii::Pipeline graphicsPipeline_;

  vk::raii::CommandPool commandPool_;
  vk::raii::CommandBuffers commandBuffers_;
  std::vector<CommandBufferInfo> commandBuffersInfo_;

  VmaAllocator allocator_;
  VkBuffer vertexBuffer_;
  VmaAllocation vertexAllocation_;

  uint32_t commandBufferIndex_ = 0;
  bool framebufferResized_ = false;
};

}  // namespace luanaut

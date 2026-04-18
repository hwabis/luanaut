#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
#include <SDL3/SDL_vulkan.h>
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
    std::vector<vk::Image> images;
    vk::SurfaceFormatKHR format;
    vk::Extent2D extent;
    std::vector<vk::raii::ImageView> imageViews;
  };

  struct SyncBundle {
    std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
    std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
    std::vector<vk::raii::Fence> inFlightFences;
  };

  auto recordCommandBuffer(uint32_t frameIndex, uint32_t imageIndex) -> void;
  auto transitionImageLayout(uint32_t frameIndex,
                             uint32_t imageIndex,
                             vk::ImageLayout oldLayout,
                             vk::ImageLayout newLayout,
                             vk::AccessFlags2 srcAccessMask,
                             vk::AccessFlags2 dstAccessMask,
                             vk::PipelineStageFlags2 srcStageMask,
                             vk::PipelineStageFlags2 dstStageMask) -> void;
  auto recreateSwapchain() -> void;

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
  static auto createSyncBundle(const vk::raii::Device& device,
                               const SwapchainBundle& swapchainBundle)
      -> SyncBundle;

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

  SyncBundle syncBundle_;

  uint32_t commandBufferIndex_ = 0;
  bool framebufferResized_ = false;
};

}  // namespace luanaut

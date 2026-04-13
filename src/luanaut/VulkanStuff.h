#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <SDL3/SDL_vulkan.h>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan_raii.hpp>

namespace luanaut {

class VulkanStuff {
 public:
  VulkanStuff(SDL_Window* window);

 private:
  struct SwapchainBundle {
    vk::raii::SwapchainKHR swapchain = nullptr;
    std::vector<vk::Image> images;
    vk::SurfaceFormatKHR format;
    vk::Extent2D extent;
    std::vector<vk::raii::ImageView> imageViews;
  };

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
  static auto createGraphicsQueue(
      const vk::raii::SurfaceKHR& surface,
      const vk::raii::PhysicalDevice& physicalDevice,
      const vk::raii::Device& device) -> vk::raii::Queue;
  static auto findGraphicsQueueIndex(
      const vk::raii::SurfaceKHR& surface,
      const vk::raii::PhysicalDevice& physicalDevice) -> uint32_t;
  static auto createSwapchainBundle(
      SDL_Window* window,
      const vk::raii::SurfaceKHR& surface,
      const vk::raii::PhysicalDevice& physicalDevice,
      const vk::raii::Device& device) -> SwapchainBundle;
  static auto createPipelineLayout(const vk::raii::Device& device)
      -> vk::raii::PipelineLayout;
  static auto createGraphicsPipeline(const vk::raii::Device& device,
                                     const SwapchainBundle& swapchainBundle,
                                     const vk::raii::PipelineLayout& layout)
      -> vk::raii::Pipeline;
  static auto readFile(const std::string& filename) -> std::vector<char>;

  SDL_Window* window_;
  vk::raii::Context context_;
  vk::raii::Instance instance_ = nullptr;
  vk::raii::DebugUtilsMessengerEXT debugMessenger_ = nullptr;
  vk::raii::SurfaceKHR surface_ = nullptr;
  vk::raii::PhysicalDevice physicalDevice_ = nullptr;
  vk::raii::Device device_ = nullptr;
  vk::raii::Queue graphicsQueue_ = nullptr;
  SwapchainBundle swapchainBundle_;
  vk::raii::PipelineLayout pipelineLayout_ = nullptr;
  vk::raii::Pipeline graphicsPipeline_ = nullptr;
};

}  // namespace luanaut

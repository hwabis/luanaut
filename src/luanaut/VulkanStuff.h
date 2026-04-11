#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <SDL3/SDL_vulkan.h>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan_raii.hpp>

namespace luanaut {

class VulkanStuff {
 public:
  VulkanStuff(SDL_Window* window);

 private:
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
  static auto createPhysicalDevice(const vk::raii::Instance& instance)
      -> vk::raii::PhysicalDevice;
  static auto createLogicalDevice(
      const vk::raii::PhysicalDevice& physicalDevice) -> vk::raii::Device;
  static auto createGraphicsQueue(
      const vk::raii::PhysicalDevice& physicalDevice,
      const vk::raii::Device& logicalDevice) -> vk::raii::Queue;
  static auto findGraphicsQueueIndex(
      const vk::raii::PhysicalDevice& physicalDevice) -> uint32_t;

  vk::raii::Context context_;
  vk::raii::Instance instance_ = nullptr;
  vk::raii::DebugUtilsMessengerEXT debugMessenger_ = nullptr;
  vk::raii::PhysicalDevice physicalDevice_ = nullptr;
  vk::raii::Device device_ = nullptr;
  vk::raii::Queue graphicsQueue_ = nullptr;
  SDL_Window* window_;
};

}  // namespace luanaut

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <SDL3/SDL_vulkan.h>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan_raii.hpp>

namespace luanaut {

class VulkanStuff {
 public:
  VulkanStuff(SDL_Window* window);

 private:
  auto createInstance() -> vk::raii::Instance;
  auto createDebugMessenger() -> vk::raii::DebugUtilsMessengerEXT;
  static VKAPI_ATTR vk::Bool32 VKAPI_CALL
  // NOLINTNEXTLINE(modernize-use-trailing-return-type)
  debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                vk::DebugUtilsMessageTypeFlagsEXT messageTypes,
                const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* /*pUserData*/);

  vk::raii::Context context_;
  vk::raii::Instance instance_ = nullptr;
  vk::raii::DebugUtilsMessengerEXT debugMessenger_ = nullptr;
  SDL_Window* window_;
};

}  // namespace luanaut

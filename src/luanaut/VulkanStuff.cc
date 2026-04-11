#include "VulkanStuff.h"

namespace luanaut {

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

VulkanStuff::VulkanStuff(SDL_Window* window)
    : instance_(createInstance()),
      debugMessenger_(createDebugMessenger()),
      window_(window) {
  window_ = nullptr;  // todo rm
}

auto VulkanStuff::createInstance() -> vk::raii::Instance {
  constexpr vk::ApplicationInfo appInfo{
      .pApplicationName = "Hello Triangle",
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = vk::ApiVersion14};

  std::vector<char const*> requiredLayers;
  if (enableValidationLayers) {
    const std::vector<char const*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};
    requiredLayers.assign(validationLayers.begin(), validationLayers.end());
  }
  auto layerProperties = context_.enumerateInstanceLayerProperties();
  auto unsupportedLayerIt = std::ranges::find_if(
      requiredLayers, [&layerProperties](auto const& requiredLayer) {
        return std::ranges::none_of(
            layerProperties, [requiredLayer](auto const& layerProperty) {
              return strcmp(layerProperty.layerName, requiredLayer) == 0;
            });
      });
  if (unsupportedLayerIt != requiredLayers.end()) {
    throw std::runtime_error("Required layer not supported: " +
                             std::string(*unsupportedLayerIt));
  }

  uint32_t sdlExtensionCount = 0;
  const char* const* sdlExtensions =
      SDL_Vulkan_GetInstanceExtensions(&sdlExtensionCount);
  if (sdlExtensions == nullptr) {
    throw std::runtime_error("Could not get SDL3 Vulkan extensions: " +
                             std::string(SDL_GetError()));
  }
  std::vector requiredExtensions(sdlExtensions,
                                 sdlExtensions + sdlExtensionCount);
  if (enableValidationLayers) {
    requiredExtensions.push_back(vk::EXTDebugUtilsExtensionName);
  }

  auto extensionProperties = context_.enumerateInstanceExtensionProperties();
  auto unsupportedPropertyIt = std::ranges::find_if(
      requiredExtensions,
      [&extensionProperties](auto const& requiredExtension) {
        return std::ranges::none_of(
            extensionProperties,
            [requiredExtension](auto const& extensionProperty) {
              return strcmp(extensionProperty.extensionName,
                            requiredExtension) == 0;
            });
      });
  if (unsupportedPropertyIt != requiredExtensions.end()) {
    throw std::runtime_error("Required extension not supported: " +
                             std::string(*unsupportedPropertyIt));
  }

  vk::InstanceCreateInfo createInfo{
      .pApplicationInfo = &appInfo,
      .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
      .ppEnabledLayerNames = requiredLayers.data(),
      .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
      .ppEnabledExtensionNames = requiredExtensions.data()};

  return {context_, createInfo};
}

auto VulkanStuff::createDebugMessenger() -> vk::raii::DebugUtilsMessengerEXT {
  if (!enableValidationLayers) {
    return nullptr;
  }

  vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
  vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
      .messageSeverity = severityFlags,
      .messageType = messageTypeFlags,
      .pfnUserCallback = &debugCallback};
  return instance_.createDebugUtilsMessengerEXT(
      debugUtilsMessengerCreateInfoEXT);
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
// NOLINTNEXTLINE(modernize-use-trailing-return-type)
VulkanStuff::debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
#pragma clang diagnostic pop
    vk::DebugUtilsMessageTypeFlagsEXT messageTypes,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* /*pUserData*/) {
  auto message = fmt::format("[Vulkan] {}: {}", vk::to_string(messageTypes),
                             pCallbackData->pMessage);

  if (messageSeverity >= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError) {
    spdlog::error(message);
  } else if (messageSeverity >=
             vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning) {
    spdlog::warn(message);
  } else if (messageSeverity >=
             vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo) {
    spdlog::info(message);
  } else {
    spdlog::debug(message);
  }

  return vk::False;
}

}  // namespace luanaut

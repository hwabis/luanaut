#include "VulkanStuff.h"
#include <vulkan/vulkan_raii.hpp>

namespace luanaut {

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<const char*> requiredDeviceExtension = {
    vk::KHRSwapchainExtensionName};

VulkanStuff::VulkanStuff(SDL_Window* window)
    : instance_(createInstance(context_)),
      debugMessenger_(createDebugMessenger(instance_)),
      physicalDevice_(createPhysicalDevice(instance_)),
      device_(createLogicalDevice(physicalDevice_)),
      graphicsQueue_(createGraphicsQueue(physicalDevice_, device_)),
      window_(window) {
  window_ = nullptr;  // todo rm
}

auto VulkanStuff::createInstance(const vk::raii::Context& context)
    -> vk::raii::Instance {
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
  auto layerProperties = context.enumerateInstanceLayerProperties();
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

  auto extensionProperties = context.enumerateInstanceExtensionProperties();
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

  return {context, createInfo};
}

auto VulkanStuff::createDebugMessenger(const vk::raii::Instance& instance)
    -> vk::raii::DebugUtilsMessengerEXT {
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
  return instance.createDebugUtilsMessengerEXT(
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

auto VulkanStuff::createPhysicalDevice(const vk::raii::Instance& instance)
    -> vk::raii::PhysicalDevice {
  auto physicalDevices = instance.enumeratePhysicalDevices();
  auto const deviceIter =
      std::ranges::find_if(physicalDevices, [&](auto const& physicalDevice) {
        bool supportsVulkan1_3 =
            physicalDevice.getProperties().apiVersion >= vk::ApiVersion13;

        auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        bool supportsGraphics =
            std::ranges::any_of(queueFamilies, [](auto const& qfp) {
              return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics);
            });

        auto availableDeviceExtensions =
            physicalDevice.enumerateDeviceExtensionProperties();
        bool supportsAllRequiredExtensions = std::ranges::all_of(
            requiredDeviceExtension,
            [&availableDeviceExtensions](auto const& requiredDeviceExtension) {
              return std::ranges::any_of(
                  availableDeviceExtensions,
                  [requiredDeviceExtension](
                      auto const& availableDeviceExtension) {
                    return strcmp(availableDeviceExtension.extensionName,
                                  requiredDeviceExtension) == 0;
                  });
            });

        auto features = physicalDevice.template getFeatures2<
            vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features,
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
        bool supportsRequiredFeatures =
            features.template get<vk::PhysicalDeviceVulkan13Features>()
                .dynamicRendering &&
            features
                .template get<
                    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
                .extendedDynamicState;

        return supportsVulkan1_3 && supportsGraphics &&
               supportsAllRequiredExtensions && supportsRequiredFeatures;
      });
  if (deviceIter == physicalDevices.end()) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }

  return *deviceIter;
}

auto VulkanStuff::createLogicalDevice(
    const vk::raii::PhysicalDevice& physicalDevice) -> vk::raii::Device {
  constexpr float queuePriority = 0.5F;
  vk::DeviceQueueCreateInfo deviceQueueCreateInfo{
      .queueFamilyIndex = findGraphicsQueueIndex(physicalDevice),
      .queueCount = 1,
      .pQueuePriorities = &queuePriority};

  vk::StructureChain<vk::PhysicalDeviceFeatures2,
                     vk::PhysicalDeviceVulkan13Features,
                     vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
      featureChain = {
          {}, {.dynamicRendering = 1U}, {.extendedDynamicState = 1U}};

  vk::DeviceCreateInfo deviceCreateInfo{
      .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &deviceQueueCreateInfo,
      .enabledExtensionCount =
          static_cast<uint32_t>(requiredDeviceExtension.size()),
      .ppEnabledExtensionNames = requiredDeviceExtension.data()};

  return {physicalDevice, deviceCreateInfo};
}

auto VulkanStuff::createGraphicsQueue(
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& logicalDevice) -> vk::raii ::Queue {
  return {logicalDevice, findGraphicsQueueIndex(physicalDevice), 0};
}

auto VulkanStuff::findGraphicsQueueIndex(
    const vk::raii::PhysicalDevice& physicalDevice) -> uint32_t {
  auto qfps = physicalDevice.getQueueFamilyProperties();
  auto iter = std::ranges::find_if(qfps, [](auto const& qfp) {
    return (qfp.queueFlags & vk::QueueFlagBits::eGraphics) !=
           static_cast<vk::QueueFlags>(0);
  });
  if (iter == qfps.end()) {
    throw std::runtime_error("No graphics queue family found!");
  }
  return static_cast<uint32_t>(std::distance(qfps.begin(), iter));
}

}  // namespace luanaut

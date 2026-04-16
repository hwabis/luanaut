#include "VulkanStuff.h"
#include <spdlog/spdlog.h>
#include <array>
#include <fstream>

namespace luanaut {

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<const char*> requiredDeviceExtensions = {
    vk::KHRSwapchainExtensionName};

VulkanStuff::VulkanStuff(SDL_Window* window)
    : window_(window),
      instance_(createInstance(context_)),
      debugMessenger_(createDebugMessenger(instance_)),
      surface_(createSurface(window_, instance_)),
      physicalDevice_(createPhysicalDevice(instance_)),
      device_(createLogicalDevice(surface_, physicalDevice_)),
      graphicsQueue_(createGraphicsQueue(surface_, physicalDevice_, device_)),
      swapchainBundle_(
          createSwapchainBundle(window_, surface_, physicalDevice_, device_)),
      pipelineLayout_(createPipelineLayout(device_)),
      graphicsPipeline_(
          createGraphicsPipeline(device_, swapchainBundle_, pipelineLayout_)),
      commandPool_(createCommandPool(surface_, device_, physicalDevice_)),
      commandBuffer_(createCommandBuffer(device_, commandPool_)) {}

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

auto VulkanStuff::createSurface(SDL_Window* window,
                                const vk::raii::Instance& instance)
    -> vk::raii::SurfaceKHR {
  VkSurfaceKHR rawSurface;

  if (!SDL_Vulkan_CreateSurface(window, *instance, nullptr, &rawSurface)) {
    throw std::runtime_error("Failed to create surface: " +
                             std::string(SDL_GetError()));
  }

  return {instance, rawSurface};
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
            requiredDeviceExtensions,
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
    const vk::raii::SurfaceKHR& surface,
    const vk::raii::PhysicalDevice& physicalDevice) -> vk::raii::Device {
  constexpr float queuePriority = 0.5F;
  vk::DeviceQueueCreateInfo deviceQueueCreateInfo{
      .queueFamilyIndex = findGraphicsQueueIndex(surface, physicalDevice),
      .queueCount = 1,
      .pQueuePriorities = &queuePriority};

  vk::StructureChain<vk::PhysicalDeviceFeatures2,
                     vk::PhysicalDeviceVulkan11Features,
                     vk::PhysicalDeviceVulkan13Features,
                     vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
      featureChain = {{},
                      {.shaderDrawParameters = vk::True},
                      {.dynamicRendering = vk::True},
                      {.extendedDynamicState = vk::True}};

  vk::DeviceCreateInfo deviceCreateInfo{
      .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &deviceQueueCreateInfo,
      .enabledExtensionCount =
          static_cast<uint32_t>(requiredDeviceExtensions.size()),
      .ppEnabledExtensionNames = requiredDeviceExtensions.data()};

  return {physicalDevice, deviceCreateInfo};
}

auto VulkanStuff::createGraphicsQueue(
    const vk::raii::SurfaceKHR& surface,
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& device) -> vk::raii::Queue {
  return {device, findGraphicsQueueIndex(surface, physicalDevice), 0};
}

auto VulkanStuff::findGraphicsQueueIndex(
    const vk::raii::SurfaceKHR& surface,
    const vk::raii::PhysicalDevice& physicalDevice) -> uint32_t {
  auto qfps = physicalDevice.getQueueFamilyProperties();

  for (uint32_t i = 0; i < qfps.size(); i++) {
    if ((qfps[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
        (physicalDevice.getSurfaceSupportKHR(i, surface) == vk::True)) {
      return i;
    }
  }

  throw std::runtime_error(
      "Could not find a queue that supports both graphics and present!");
}

auto VulkanStuff::createSwapchainBundle(
    SDL_Window* window,
    const vk::raii::SurfaceKHR& surface,
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& device) -> SwapchainBundle {
  vk::SurfaceCapabilitiesKHR capabilities =
      physicalDevice.getSurfaceCapabilitiesKHR(*surface);

  auto minImageCount = std::max(3U, capabilities.minImageCount);
  if (capabilities.maxImageCount > 0 &&
      (capabilities.maxImageCount < minImageCount)) {
    minImageCount = capabilities.maxImageCount;
  }

  std::vector<vk::SurfaceFormatKHR> availableFormats =
      physicalDevice.getSurfaceFormatsKHR(*surface);
  const auto formatIt =
      std::ranges::find_if(availableFormats, [](const auto& format) {
        return format.format == vk::Format::eB8G8R8A8Srgb &&
               format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
      });
  auto swapchainSurfaceFormat =
      formatIt != availableFormats.end() ? *formatIt : availableFormats[0];

  vk::Extent2D swapchainExtent;
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    swapchainExtent = capabilities.currentExtent;
  } else {
    int width;
    int height;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    swapchainExtent = {
        .width = std::clamp<uint32_t>(width, capabilities.minImageExtent.width,
                                      capabilities.maxImageExtent.width),
        .height =
            std::clamp<uint32_t>(height, capabilities.minImageExtent.height,
                                 capabilities.maxImageExtent.height)};
  }

  std::vector<vk::PresentModeKHR> availablePresentModes =
      physicalDevice.getSurfacePresentModesKHR(*surface);
  auto presentMode =
      std::ranges::any_of(availablePresentModes,
                          [](const vk::PresentModeKHR value) {
                            return value == vk::PresentModeKHR::eMailbox;
                          })
          ? vk::PresentModeKHR::eMailbox
          : vk::PresentModeKHR::eFifo;

  vk::SwapchainCreateInfoKHR swapChainCreateInfo{
      .surface = *surface,
      .minImageCount = minImageCount,
      .imageFormat = swapchainSurfaceFormat.format,
      .imageColorSpace = swapchainSurfaceFormat.colorSpace,
      .imageExtent = swapchainExtent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .imageSharingMode = vk::SharingMode::eExclusive,
      .preTransform = capabilities.currentTransform,
      .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
      .presentMode = presentMode,
      .clipped = vk::True};

  vk::raii::SwapchainKHR swapchain(device, swapChainCreateInfo);
  auto images = swapchain.getImages();

  vk::ImageViewCreateInfo imageViewCreateInfo{
      .viewType = vk::ImageViewType::e2D,
      .format = swapchainSurfaceFormat.format,
      .subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor,
                           .baseMipLevel = 0,
                           .levelCount = 1,
                           .baseArrayLayer = 0,
                           .layerCount = 1}};

  std::vector<vk::raii::ImageView> imageViews;
  for (auto& image : images) {
    imageViewCreateInfo.image = image;
    imageViews.emplace_back(device, imageViewCreateInfo);
  }

  return {.swapchain = std::move(swapchain),
          .images = std::move(images),
          .format = swapchainSurfaceFormat,
          .extent = swapchainExtent,
          .imageViews = std::move(imageViews)};
}

auto VulkanStuff::createPipelineLayout(const vk::raii::Device& device)
    -> vk::raii::PipelineLayout {
  vk::PipelineLayoutCreateInfo pipelineLayoutInfo{.setLayoutCount = 0,
                                                  .pushConstantRangeCount = 0};
  return {device, pipelineLayoutInfo};
}

auto VulkanStuff::createGraphicsPipeline(const vk::raii::Device& device,
                                         const SwapchainBundle& swapchainBundle,
                                         const vk::raii::PipelineLayout& layout)
    -> vk::raii::Pipeline {
  auto shaderCode = readFile(SHADER_PATH);
  vk::ShaderModuleCreateInfo createInfo{
      .codeSize = shaderCode.size() * sizeof(char),
      .pCode = reinterpret_cast<const uint32_t*>(shaderCode.data())};
  vk::raii::ShaderModule shaderModule{device, createInfo};

  vk::PipelineShaderStageCreateInfo vertShaderStageInfo{
      .stage = vk::ShaderStageFlagBits::eVertex,
      .module = shaderModule,
      .pName = "vertMain"};
  vk::PipelineShaderStageCreateInfo fragShaderStageInfo{
      .stage = vk::ShaderStageFlagBits::eFragment,
      .module = shaderModule,
      .pName = "fragMain"};
  std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {
      vertShaderStageInfo, fragShaderStageInfo};

  vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

  vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
      .topology = vk::PrimitiveTopology::eTriangleList};

  vk::Viewport viewport{
      .x = 0.0F,
      .y = 0.0F,
      .width = static_cast<float>(swapchainBundle.extent.width),
      .height = static_cast<float>(swapchainBundle.extent.height),
      .minDepth = 0.0F,
      .maxDepth = 1.0F};
  vk::Rect2D scissor{.offset = vk::Offset2D{.x = 0, .y = 0},
                     .extent = swapchainBundle.extent};
  vk::PipelineViewportStateCreateInfo viewportState{.viewportCount = 1,
                                                    .pViewports = &viewport,
                                                    .scissorCount = 1,
                                                    .pScissors = &scissor};

  vk::PipelineRasterizationStateCreateInfo rasterizer{
      .depthClampEnable = vk::False,
      .rasterizerDiscardEnable = vk::False,
      .polygonMode = vk::PolygonMode::eFill,
      .cullMode = vk::CullModeFlagBits::eBack,
      .frontFace = vk::FrontFace::eClockwise,
      .depthBiasEnable = vk::False,
      .lineWidth = 1.0F};

  vk::PipelineMultisampleStateCreateInfo multisampling{
      .rasterizationSamples = vk::SampleCountFlagBits::e1,
      .sampleShadingEnable = vk::False};

  vk::PipelineColorBlendAttachmentState colorBlendAttachment{
      .blendEnable = vk::True,
      .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
      .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
      .colorBlendOp = vk::BlendOp::eAdd,
      .srcAlphaBlendFactor = vk::BlendFactor::eOne,
      .dstAlphaBlendFactor = vk::BlendFactor::eZero,
      .alphaBlendOp = vk::BlendOp::eAdd,
      .colorWriteMask =
          vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};
  vk::PipelineColorBlendStateCreateInfo colorBlending{
      .logicOpEnable = vk::False,
      .logicOp = vk::LogicOp::eCopy,
      .attachmentCount = 1,
      .pAttachments = &colorBlendAttachment};

  std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport,
                                                 vk::DynamicState::eScissor};
  vk::PipelineDynamicStateCreateInfo dynamicState{
      .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
      .pDynamicStates = dynamicStates.data()};

  vk::GraphicsPipelineCreateInfo pipelineInfo{
      .stageCount = 2,
      .pStages = shaderStages.data(),
      .pVertexInputState = &vertexInputInfo,
      .pInputAssemblyState = &inputAssembly,
      .pViewportState = &viewportState,
      .pRasterizationState = &rasterizer,
      .pMultisampleState = &multisampling,
      .pColorBlendState = &colorBlending,
      .pDynamicState = &dynamicState,
      .layout = layout,
      .renderPass = nullptr};
  vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo{
      .colorAttachmentCount = 1,
      .pColorAttachmentFormats = &swapchainBundle.format.format};

  vk::StructureChain<vk::GraphicsPipelineCreateInfo,
                     vk::PipelineRenderingCreateInfo>
      pipelineCreateInfoChain = {pipelineInfo, pipelineRenderingCreateInfo};

  return {device, nullptr,
          pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>()};
}

auto VulkanStuff::readFile(const std::string& filename) -> std::vector<char> {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  std::vector<char> buffer(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
  file.close();
  return buffer;
}

auto VulkanStuff::createCommandPool(
    const vk::raii::SurfaceKHR& surface,
    const vk::raii::Device& device,
    const vk::raii::PhysicalDevice& physicalDevice) -> vk::raii::CommandPool {
  auto queueIndex = findGraphicsQueueIndex(surface, physicalDevice);
  vk::CommandPoolCreateInfo poolInfo{
      .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      .queueFamilyIndex = queueIndex};

  return {device, poolInfo};
}

auto VulkanStuff::createCommandBuffer(const vk::raii::Device& device,
                                      const vk::raii::CommandPool& commandPool)
    -> vk::raii::CommandBuffer {
  vk::CommandBufferAllocateInfo allocInfo{
      .commandPool = commandPool,
      .level = vk::CommandBufferLevel::ePrimary,
      .commandBufferCount = 1};

  return std::move(vk::raii::CommandBuffers(device, allocInfo).front());
}

}  // namespace luanaut

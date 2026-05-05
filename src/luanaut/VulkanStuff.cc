#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
#include "VulkanStuff.h"
#include <SDL3/SDL_events.h>
#include <spdlog/spdlog.h>
#include <array>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include "shaders/UniformBufferObject.h"
#include "shaders/Vertex.h"

namespace luanaut {

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<const char*> requiredDeviceExtensions = {
    vk::KHRSwapchainExtensionName};

constexpr int commandBufferCount = 2;

const std::vector<Vertex> vertices = {
    // Front (0, 0, 1)
    {.pos = {-0.5F, -0.5F, 0.5F}, .normal = {0.0F, 0.0F, 1.0F}},
    {.pos = {0.5F, -0.5F, 0.5F}, .normal = {0.0F, 0.0F, 1.0F}},
    {.pos = {0.5F, 0.5F, 0.5F}, .normal = {0.0F, 0.0F, 1.0F}},
    {.pos = {-0.5F, 0.5F, 0.5F}, .normal = {0.0F, 0.0F, 1.0F}},
    // Back (0, 0, -1)
    {.pos = {0.5F, -0.5F, -0.5F}, .normal = {0.0F, 0.0F, -1.0F}},
    {.pos = {-0.5F, -0.5F, -0.5F}, .normal = {0.0F, 0.0F, -1.0F}},
    {.pos = {-0.5F, 0.5F, -0.5F}, .normal = {0.0F, 0.0F, -1.0F}},
    {.pos = {0.5F, 0.5F, -0.5F}, .normal = {0.0F, 0.0F, -1.0F}},
    // Left (-1, 0, 0)
    {.pos = {-0.5F, -0.5F, -0.5F}, .normal = {-1.0F, 0.0F, 0.0F}},
    {.pos = {-0.5F, -0.5F, 0.5F}, .normal = {-1.0F, 0.0F, 0.0F}},
    {.pos = {-0.5F, 0.5F, 0.5F}, .normal = {-1.0F, 0.0F, 0.0F}},
    {.pos = {-0.5F, 0.5F, -0.5F}, .normal = {-1.0F, 0.0F, 0.0F}},
    // Right (1, 0, 0)
    {.pos = {0.5F, -0.5F, 0.5F}, .normal = {1.0F, 0.0F, 0.0F}},
    {.pos = {0.5F, -0.5F, -0.5F}, .normal = {1.0F, 0.0F, 0.0F}},
    {.pos = {0.5F, 0.5F, -0.5F}, .normal = {1.0F, 0.0F, 0.0F}},
    {.pos = {0.5F, 0.5F, 0.5F}, .normal = {1.0F, 0.0F, 0.0F}},
    // Top (0, 1, 0)
    {.pos = {-0.5F, 0.5F, -0.5F}, .normal = {0.0F, 1.0F, 0.0F}},
    {.pos = {-0.5F, 0.5F, 0.5F}, .normal = {0.0F, 1.0F, 0.0F}},
    {.pos = {0.5F, 0.5F, 0.5F}, .normal = {0.0F, 1.0F, 0.0F}},
    {.pos = {0.5F, 0.5F, -0.5F}, .normal = {0.0F, 1.0F, 0.0F}},
    // Bottom (0, -1, 0)
    {.pos = {-0.5F, -0.5F, 0.5F}, .normal = {0.0F, -1.0F, 0.0F}},
    {.pos = {-0.5F, -0.5F, -0.5F}, .normal = {0.0F, -1.0F, 0.0F}},
    {.pos = {0.5F, -0.5F, -0.5F}, .normal = {0.0F, -1.0F, 0.0F}},
    {.pos = {0.5F, -0.5F, 0.5F}, .normal = {0.0F, -1.0F, 0.0F}},
};

const std::vector<uint16_t> indices = {
    0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
    12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
};

VulkanStuff::VulkanStuff(SDL_Window* window)
    : window_(window),
      instance_(createInstance(context_)),
      debugMessenger_(createDebugMessenger(instance_)),
      surface_(createSurface(window_, instance_)),
      physicalDevice_(createPhysicalDevice(instance_)),
      device_(createLogicalDevice(surface_, physicalDevice_)),
      graphicsQueue_(device_,
                     findGraphicsQueueIndex(surface_, physicalDevice_),
                     0),
      swapchainBundle_(
          createSwapchainBundle(window_, surface_, physicalDevice_, device_)),
      descriptorPool_(createDescriptorPool(device_)),
      descriptorSetLayout_(createDescriptorSetLayout(device_)),
      pipelineLayout_(createPipelineLayout(device_, descriptorSetLayout_)),
      graphicsPipeline_(
          createGraphicsPipeline(device_, swapchainBundle_, pipelineLayout_)),
      allocator_(*physicalDevice_, *device_, *instance_),
      depthBundle_(createDepthBundle(physicalDevice_,
                                     device_,
                                     swapchainBundle_,
                                     allocator_)),
      commandPool_(createCommandPool(surface_, device_, physicalDevice_)),
      commandBuffers_(createCommandBuffers(device_, commandPool_)),
      commandBuffersInfo_(createCommandBuffersInfo(device_,
                                                   allocator_,
                                                   descriptorPool_,
                                                   descriptorSetLayout_)) {
  uploadVertices();
  uploadIndices();
}

VulkanStuff::~VulkanStuff() {
  device_.waitIdle();
  // todo raii wrap VmaAllocation
  for (auto& info : commandBuffersInfo_) {
    vmaDestroyBuffer(allocator_, info.uniformBuffer, info.uniformAllocation);
  }
  vmaDestroyBuffer(allocator_, vertexBuffer_, vertexAllocation_);
  vmaDestroyBuffer(allocator_, indexBuffer_, indexAllocation_);
  vmaDestroyImage(allocator_, depthBundle_.image, depthBundle_.allocation);
}

auto VulkanStuff::DrawFrame() -> void {
  auto fenceResult = device_.waitForFences(
      *commandBuffersInfo_[commandBufferIndex_].fence, vk::True, UINT64_MAX);
  if (fenceResult != vk::Result::eSuccess) {
    throw std::runtime_error("failed to wait for fence!");
  }

  auto [result, imageIndex] = swapchainBundle_.swapchain.acquireNextImage(
      UINT64_MAX,
      *commandBuffersInfo_[commandBufferIndex_].presentCompleteSemaphore,
      nullptr);
  if (result == vk::Result::eErrorOutOfDateKHR) {
    recreateSwapchain();
    return;
  }
  if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  device_.resetFences(*commandBuffersInfo_[commandBufferIndex_].fence);

  commandBuffers_[commandBufferIndex_].reset();
  uboModel_ = glm::rotate(uboModel_, glm::radians(0.5F), glm::vec3(1, 1, -1));
  constexpr float fov = 60.0F;
  auto proj =
      glm::perspective(glm::radians(fov),
                       static_cast<float>(swapchainBundle_.extent.width) /
                           static_cast<float>(swapchainBundle_.extent.height),
                       0.1F, 100.0F);
  proj[1][1] *= -1;
  UniformBufferObject ubo{
      .model = uboModel_,
      .view = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0),
                          glm::vec3(0, 1, 0)),
      .proj = proj};
  memcpy(commandBuffersInfo_[commandBufferIndex_].uniformBufferMapped, &ubo,
         sizeof(ubo));
  recordCommandBuffer(commandBuffers_[commandBufferIndex_], imageIndex);

  vk::PipelineStageFlags waitDestinationStageMask(
      vk::PipelineStageFlagBits::eColorAttachmentOutput);
  const vk::SubmitInfo submitInfo{
      .waitSemaphoreCount = 1,
      .pWaitSemaphores =
          &*commandBuffersInfo_[commandBufferIndex_].presentCompleteSemaphore,
      .pWaitDstStageMask = &waitDestinationStageMask,
      .commandBufferCount = 1,
      .pCommandBuffers = &*commandBuffers_[commandBufferIndex_],
      .signalSemaphoreCount = 1,
      .pSignalSemaphores =
          &*swapchainBundle_.imagesInfo[imageIndex].renderFinishedSemaphore};
  graphicsQueue_.submit(submitInfo,
                        *commandBuffersInfo_[commandBufferIndex_].fence);

  const vk::PresentInfoKHR presentInfoKHR{
      .waitSemaphoreCount = 1,
      .pWaitSemaphores =
          &*swapchainBundle_.imagesInfo[imageIndex].renderFinishedSemaphore,
      .swapchainCount = 1,
      .pSwapchains = &*swapchainBundle_.swapchain,
      .pImageIndices = &imageIndex};
  result = graphicsQueue_.presentKHR(presentInfoKHR);
  if (result == vk::Result::eSuboptimalKHR ||
      result == vk::Result::eErrorOutOfDateKHR || framebufferResized_) {
    framebufferResized_ = false;
    recreateSwapchain();
  }

  commandBufferIndex_ = (commandBufferIndex_ + 1) % commandBufferCount;
}

auto VulkanStuff::NotifyResize() -> void {
  framebufferResized_ = true;
}

auto VulkanStuff::recreateSwapchain() -> void {
  // SDL_GetWindowSizeInPixels does not return 0x0 on minimize T_T
  vk::SurfaceCapabilitiesKHR caps =
      physicalDevice_.getSurfaceCapabilitiesKHR(*surface_);
  if (caps.currentExtent.width == 0 || caps.currentExtent.height == 0) {
    return;
  }

  device_.waitIdle();
  swapchainBundle_ = createSwapchainBundle(window_, surface_, physicalDevice_,
                                           device_, swapchainBundle_.swapchain);
}

auto VulkanStuff::recordCommandBuffer(const vk::raii::CommandBuffer& cmd,
                                      uint32_t imageIndex) -> void {
  cmd.begin({});

  transitionImageLayout(cmd, swapchainBundle_.imagesInfo[imageIndex].image,
                        vk::PipelineStageFlagBits2::eColorAttachmentOutput, {},
                        vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                        vk::AccessFlagBits2::eColorAttachmentWrite,
                        vk::ImageLayout::eUndefined,
                        vk::ImageLayout::eColorAttachmentOptimal,
                        vk::ImageAspectFlagBits::eColor);
  transitionImageLayout(cmd, depthBundle_.image,
                        vk::PipelineStageFlagBits2::eEarlyFragmentTests |
                            vk::PipelineStageFlagBits2::eLateFragmentTests,
                        {},
                        vk::PipelineStageFlagBits2::eEarlyFragmentTests |
                            vk::PipelineStageFlagBits2::eLateFragmentTests,
                        vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
                        vk::ImageLayout::eUndefined,
                        vk::ImageLayout::eDepthAttachmentOptimal,
                        vk::ImageAspectFlagBits::eDepth);

  vk::RenderingAttachmentInfo colorAttachmentInfo{
      .imageView = *swapchainBundle_.imagesInfo[imageIndex].imageView,
      .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
      .loadOp = vk::AttachmentLoadOp::eClear,
      .storeOp = vk::AttachmentStoreOp::eStore,
      .clearValue = vk::ClearColorValue{0.0F, 0.0F, 0.0F, 1.0F}};
  vk::RenderingAttachmentInfo depthAttachmentInfo{
      .imageView = *depthBundle_.imageView,
      .imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
      .loadOp = vk::AttachmentLoadOp::eClear,
      .storeOp = vk::AttachmentStoreOp::eDontCare,
      .clearValue = vk::ClearDepthStencilValue{.depth = 1.0F, .stencil = 0}};
  vk::RenderingInfo renderingInfo{
      .renderArea = {.offset = {.x = 0, .y = 0},
                     .extent = swapchainBundle_.extent},
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &colorAttachmentInfo,
      .pDepthAttachment = &depthAttachmentInfo};

  cmd.beginRendering(renderingInfo);
  cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline_);
  cmd.setViewport(
      0, vk::Viewport(
             0.0F, 0.0F, static_cast<float>(swapchainBundle_.extent.width),
             static_cast<float>(swapchainBundle_.extent.height), 0.F, 1.F));
  cmd.setScissor(0, vk::Rect2D({.x = 0, .y = 0}, swapchainBundle_.extent));
  cmd.bindVertexBuffers(0, vk::Buffer(vertexBuffer_), {0});
  cmd.bindIndexBuffer(indexBuffer_, 0, vk::IndexType::eUint16);
  cmd.bindDescriptorSets(
      vk::PipelineBindPoint::eGraphics, *pipelineLayout_, 0,
      *commandBuffersInfo_[commandBufferIndex_].descriptorSet, {});
  cmd.drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
  cmd.endRendering();

  transitionImageLayout(cmd, swapchainBundle_.imagesInfo[imageIndex].image,
                        vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                        vk::AccessFlagBits2::eColorAttachmentWrite,
                        vk::PipelineStageFlagBits2::eBottomOfPipe, {},
                        vk::ImageLayout::eColorAttachmentOptimal,
                        vk::ImageLayout::ePresentSrcKHR,
                        vk::ImageAspectFlagBits::eColor);

  cmd.end();
}

auto VulkanStuff::transitionImageLayout(const vk::raii::CommandBuffer& cmd,
                                        vk::Image image,
                                        vk::PipelineStageFlags2 srcStageMask,
                                        vk::AccessFlags2 srcAccessMask,
                                        vk::PipelineStageFlags2 destStageMask,
                                        vk::AccessFlags2 destAccessMask,
                                        vk::ImageLayout oldLayout,
                                        vk::ImageLayout newLayout,
                                        vk::ImageAspectFlagBits aspectMask)
    -> void {
  vk::ImageMemoryBarrier2 barrier = {
      .srcStageMask = srcStageMask,
      .srcAccessMask = srcAccessMask,
      .dstStageMask = destStageMask,
      .dstAccessMask = destAccessMask,
      .oldLayout = oldLayout,
      .newLayout = newLayout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = image,
      .subresourceRange = {.aspectMask = aspectMask,
                           .baseMipLevel = 0,
                           .levelCount = 1,
                           .baseArrayLayer = 0,
                           .layerCount = 1}};
  vk::DependencyInfo dependencyInfo = {.dependencyFlags = {},
                                       .imageMemoryBarrierCount = 1,
                                       .pImageMemoryBarriers = &barrier};
  cmd.pipelineBarrier2(dependencyInfo);
}

auto VulkanStuff::createInstance(const vk::raii::Context& context)
    -> vk::raii::Instance {
  constexpr vk::ApplicationInfo appInfo{.apiVersion = vk::ApiVersion14};

  std::vector<const char*> requiredLayers;
  if (enableValidationLayers) {
    const std::vector<char const*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};
    requiredLayers.assign(validationLayers.begin(), validationLayers.end());
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
        if (physicalDevice.getProperties().apiVersion < vk::ApiVersion13) {
          return false;
        }

        auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        bool supportsGraphics =
            std::ranges::any_of(queueFamilies, [](auto const& qfp) {
              return static_cast<bool>(qfp.queueFlags &
                                       vk::QueueFlagBits::eGraphics);
            });
        if (!supportsGraphics) {
          return false;
        }

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
        if (!supportsAllRequiredExtensions) {
          return false;
        }

        auto features = physicalDevice.template getFeatures2<
            vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features,
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
        bool supportsFeatures =
            features.template get<vk::PhysicalDeviceVulkan13Features>()
                .dynamicRendering &&
            features
                .template get<
                    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
                .extendedDynamicState;

        return supportsFeatures;
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
      featureChain = {
          {},
          {.shaderDrawParameters = vk::True},
          {.synchronization2 = vk::True, .dynamicRendering = vk::True},
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
    const vk::raii::Device& device,
    vk::SwapchainKHR oldSwapchain) -> SwapchainBundle {
  vk::SurfaceCapabilitiesKHR capabilities =
      physicalDevice.getSurfaceCapabilitiesKHR(*surface);

  uint32_t imageCount = std::clamp(
      3U, capabilities.minImageCount,
      capabilities.maxImageCount > 0 ? capabilities.maxImageCount : UINT32_MAX);

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
      .minImageCount = imageCount,
      .imageFormat = swapchainSurfaceFormat.format,
      .imageColorSpace = swapchainSurfaceFormat.colorSpace,
      .imageExtent = swapchainExtent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .imageSharingMode = vk::SharingMode::eExclusive,
      .preTransform = capabilities.currentTransform,
      .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
      .presentMode = presentMode,
      .clipped = vk::True,
      .oldSwapchain = oldSwapchain};

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
  std::vector<SwapchainBundle::ImageInfo> imagesInfo;
  for (auto& image : images) {
    imageViewCreateInfo.image = image;
    imagesInfo.push_back(SwapchainBundle::ImageInfo{
        .image = image,
        .imageView = {device, imageViewCreateInfo},
        .renderFinishedSemaphore = {device, vk::SemaphoreCreateInfo()},
    });
  }

  return {
      .swapchain = std::move(swapchain),
      .imagesInfo = std::move(imagesInfo),
      .format = swapchainSurfaceFormat,
      .extent = swapchainExtent,
  };
}

auto VulkanStuff::createDescriptorPool(const vk::raii::Device& device)
    -> vk::raii::DescriptorPool {
  vk::DescriptorPoolSize poolSize{.type = vk::DescriptorType::eUniformBuffer,
                                  .descriptorCount = commandBufferCount};
  vk::DescriptorPoolCreateInfo poolInfo{
      .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
      .maxSets = commandBufferCount,
      .poolSizeCount = 1,
      .pPoolSizes = &poolSize};

  return {device, poolInfo};
}

auto VulkanStuff::createDescriptorSetLayout(const vk::raii::Device& device)
    -> vk::raii::DescriptorSetLayout {
  vk::DescriptorSetLayoutBinding layoutBinding{
      .binding = 0,
      .descriptorType = vk::DescriptorType::eUniformBuffer,
      .descriptorCount = 1,
      .stageFlags = vk::ShaderStageFlagBits::eVertex};
  vk::DescriptorSetLayoutCreateInfo layoutInfo{.bindingCount = 1,
                                               .pBindings = &layoutBinding};

  return {device, layoutInfo};
}

auto VulkanStuff::createPipelineLayout(
    const vk::raii::Device& device,
    const vk::raii::DescriptorSetLayout& descriptorSetLayout)
    -> vk::raii::PipelineLayout {
  return {
      device,
      vk::PipelineLayoutCreateInfo{.setLayoutCount = 1,
                                   .pSetLayouts = &*descriptorSetLayout},
  };
}

auto VulkanStuff::createGraphicsPipeline(const vk::raii::Device& device,
                                         const SwapchainBundle& swapchainBundle,
                                         const vk::raii::PipelineLayout& layout)
    -> vk::raii::Pipeline {
  auto shaderSpirV = readFile(SHADER_PATH);
  vk::ShaderModuleCreateInfo createInfo{
      .codeSize = shaderSpirV.size() * sizeof(uint32_t),
      .pCode = shaderSpirV.data()};
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

  auto bindingDescription = Vertex::GetBindingDescription();
  auto attributeDescriptions = Vertex::GetAttributeDescriptions();
  vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &bindingDescription,
      .vertexAttributeDescriptionCount =
          static_cast<uint32_t>(attributeDescriptions.size()),
      .pVertexAttributeDescriptions = attributeDescriptions.data()};

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
      .frontFace = vk::FrontFace::eCounterClockwise,
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

  vk::PipelineDepthStencilStateCreateInfo depthStencil{
      .depthTestEnable = vk::True,
      .depthWriteEnable = vk::True,
      .depthCompareOp = vk::CompareOp::eLess,
      .depthBoundsTestEnable = vk::False,
      .stencilTestEnable = vk::False};

  vk::GraphicsPipelineCreateInfo pipelineInfo{
      .stageCount = 2,
      .pStages = shaderStages.data(),
      .pVertexInputState = &vertexInputInfo,
      .pInputAssemblyState = &inputAssembly,
      .pViewportState = &viewportState,
      .pRasterizationState = &rasterizer,
      .pMultisampleState = &multisampling,
      .pDepthStencilState = &depthStencil,
      .pColorBlendState = &colorBlending,
      .pDynamicState = &dynamicState,
      .layout = layout,
      .renderPass = nullptr};
  vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo{
      .colorAttachmentCount = 1,
      .pColorAttachmentFormats = &swapchainBundle.format.format,
      .depthAttachmentFormat = vk::Format::eD32Sfloat};

  vk::StructureChain<vk::GraphicsPipelineCreateInfo,
                     vk::PipelineRenderingCreateInfo>
      pipelineCreateInfoChain = {pipelineInfo, pipelineRenderingCreateInfo};

  return {device, nullptr,
          pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>()};
}

auto VulkanStuff::readFile(const std::string& filename)
    -> std::vector<uint32_t> {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open file: " + filename);
  }

  size_t fileSize = file.tellg();
  std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
  file.seekg(0);
  file.read(reinterpret_cast<char*>(buffer.data()),
            static_cast<std::streamsize>(fileSize));
  return buffer;
}

auto VulkanStuff::createDepthBundle(
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& device,
    const SwapchainBundle& swapchainBundle,
    const VmaAllocatorHandle& allocator) -> DepthBundle {
  std::vector<vk::Format> formatCandidates{vk::Format::eD32Sfloat,
                                           vk::Format::eD32SfloatS8Uint,
                                           vk::Format::eD24UnormS8Uint};
  auto formatItr = std::ranges::find_if(
      formatCandidates, [&physicalDevice](vk::Format candidate) {
        auto props = physicalDevice.getFormatProperties(candidate);
        return (props.optimalTilingFeatures &
                vk::FormatFeatureFlagBits::eDepthStencilAttachment) ==
               vk::FormatFeatureFlagBits::eDepthStencilAttachment;
      });
  if (formatItr == formatCandidates.end()) {
    throw std::runtime_error("failed to find supported format!");
  }
  vk::Format depthFormat = *formatItr;

  VkImageCreateInfo imageInfo{
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .imageType = VK_IMAGE_TYPE_2D,
      .format = static_cast<VkFormat>(depthFormat),
      .extent = {swapchainBundle.extent.width, swapchainBundle.extent.height,
                 1},
      .mipLevels = 1,
      .arrayLayers = 1,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .tiling = VK_IMAGE_TILING_OPTIMAL,
      .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = nullptr,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
  };
  VmaAllocationCreateInfo allocInfo{
      .flags = 0,
      .usage = VMA_MEMORY_USAGE_GPU_ONLY,
      .requiredFlags = 0,
      .preferredFlags = 0,
      .memoryTypeBits = 0,
      .pool = nullptr,
      .pUserData = nullptr,
      .priority = 0.0F,
  };
  VkImage depthImage;
  VmaAllocation depthAllocation;
  vmaCreateImage(allocator, &imageInfo, &allocInfo, &depthImage,
                 &depthAllocation, nullptr);

  vk::ImageViewCreateInfo viewInfo{
      .image = depthImage,
      .viewType = vk::ImageViewType::e2D,
      .format = depthFormat,
      .subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eDepth,
                           .baseMipLevel = 0,
                           .levelCount = 1,
                           .baseArrayLayer = 0,
                           .layerCount = 1}};
  vk::raii::ImageView depthImageView = vk::raii::ImageView{device, viewInfo};

  return {
      .image = vk::Image{depthImage},
      .allocation = depthAllocation,
      .imageView = std::move(depthImageView),
  };
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

auto VulkanStuff::createCommandBuffers(const vk::raii::Device& device,
                                       const vk::raii::CommandPool& commandPool)
    -> vk::raii::CommandBuffers {
  vk::CommandBufferAllocateInfo allocInfo{
      .commandPool = commandPool,
      .level = vk::CommandBufferLevel::ePrimary,
      .commandBufferCount = commandBufferCount};
  return {device, allocInfo};
}

auto VulkanStuff::createCommandBuffersInfo(
    const vk::raii::Device& device,
    VmaAllocator allocator,
    const vk::raii::DescriptorPool& pool,
    const vk::raii::DescriptorSetLayout& layout)
    -> std::vector<CommandBufferInfo> {
  std::vector<CommandBufferInfo> infos;
  infos.reserve(commandBufferCount);

  VkBufferCreateInfo bufferInfo{.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                                .pNext = nullptr,
                                .flags = 0,
                                .size = sizeof(UniformBufferObject),
                                .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                .queueFamilyIndexCount = 0,
                                .pQueueFamilyIndices = nullptr};
  VmaAllocationCreateInfo allocInfo{.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
                                    .usage = VMA_MEMORY_USAGE_CPU_TO_GPU,
                                    .requiredFlags = 0,
                                    .preferredFlags = 0,
                                    .memoryTypeBits = 0,
                                    .pool = nullptr,
                                    .pUserData = nullptr,
                                    .priority = 0.0F};
  std::vector<vk::DescriptorSetLayout> layouts(commandBufferCount, *layout);
  vk::DescriptorSetAllocateInfo setInfo{
      .descriptorPool = *pool,
      .descriptorSetCount = commandBufferCount,
      .pSetLayouts = layouts.data()};
  auto allSets = vk::raii::DescriptorSets(device, setInfo);

  for (size_t i = 0; i < commandBufferCount; i++) {
    CommandBufferInfo info{
        .fence = {device,
                  vk::FenceCreateInfo{.flags =
                                          vk::FenceCreateFlagBits::eSignaled}},
        .presentCompleteSemaphore = {device, vk::SemaphoreCreateInfo()},
        .uniformBuffer = nullptr,
        .uniformAllocation = nullptr,
        .uniformBufferMapped = nullptr,
        .descriptorSet = std::move(allSets[i])};

    VmaAllocationInfo allocationInfo;
    vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &info.uniformBuffer,
                    &info.uniformAllocation, &allocationInfo);
    info.uniformBufferMapped = allocationInfo.pMappedData;

    vk::DescriptorBufferInfo bufferDescInfo{
        .buffer = info.uniformBuffer,
        .offset = 0,
        .range = sizeof(UniformBufferObject)};
    vk::WriteDescriptorSet descriptorWrite{
        .dstSet = *info.descriptorSet,
        .dstBinding = 0,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = vk::DescriptorType::eUniformBuffer,
        .pBufferInfo = &bufferDescInfo};
    device.updateDescriptorSets(descriptorWrite, nullptr);

    infos.push_back(std::move(info));
  }

  return infos;
}

auto VulkanStuff::uploadVertices() -> void {
  VkBuffer stagingBuffer;
  VmaAllocation stagingAllocation;
  VkBufferCreateInfo stagingInfo{
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .size = sizeof(Vertex) * vertices.size(),
      .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = nullptr,
  };
  VmaAllocationCreateInfo stagingAllocInfo{
      .flags = 0,
      .usage = VMA_MEMORY_USAGE_CPU_ONLY,
      .requiredFlags = 0,
      .preferredFlags = 0,
      .memoryTypeBits = 0,
      .pool = nullptr,
      .pUserData = nullptr,
      .priority = 0.0F,
  };
  vmaCreateBuffer(allocator_, &stagingInfo, &stagingAllocInfo, &stagingBuffer,
                  &stagingAllocation, nullptr);
  void* data;
  vmaMapMemory(allocator_, stagingAllocation, &data);
  memcpy(data, vertices.data(), sizeof(Vertex) * vertices.size());
  vmaUnmapMemory(allocator_, stagingAllocation);

  VkBufferCreateInfo vertexBufferInfo = stagingInfo;
  vertexBufferInfo.usage =
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  VmaAllocationCreateInfo vertexAllocInfo = stagingAllocInfo;
  vertexAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
  vmaCreateBuffer(allocator_, &vertexBufferInfo, &vertexAllocInfo,
                  &vertexBuffer_, &vertexAllocation_, nullptr);

  vk::CommandBufferAllocateInfo allocInfo{
      .commandPool = commandPool_,
      .level = vk::CommandBufferLevel::ePrimary,
      .commandBufferCount = 1};
  auto transferCmdBuf =
      std::move(vk::raii::CommandBuffers(device_, allocInfo).front());
  transferCmdBuf.begin(
      {.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
  transferCmdBuf.copyBuffer(
      stagingBuffer, vertexBuffer_,
      vk::BufferCopy{.size = sizeof(Vertex) * vertices.size()});
  transferCmdBuf.end();

  vk::SubmitInfo submitInfo{.commandBufferCount = 1,
                            .pCommandBuffers = &*transferCmdBuf};
  graphicsQueue_.submit(submitInfo);

  graphicsQueue_.waitIdle();
  vmaDestroyBuffer(allocator_, stagingBuffer, stagingAllocation);
}

auto VulkanStuff::uploadIndices() -> void {
  VkBuffer stagingBuffer;
  VmaAllocation stagingAllocation;
  VkBufferCreateInfo stagingInfo{
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .size = sizeof(uint16_t) * indices.size(),
      .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = nullptr,
  };

  VmaAllocationCreateInfo stagingAllocInfo{
      .flags = 0,
      .usage = VMA_MEMORY_USAGE_CPU_ONLY,
      .requiredFlags = 0,
      .preferredFlags = 0,
      .memoryTypeBits = 0,
      .pool = nullptr,
      .pUserData = nullptr,
      .priority = 0.0F,
  };

  vmaCreateBuffer(allocator_, &stagingInfo, &stagingAllocInfo, &stagingBuffer,
                  &stagingAllocation, nullptr);
  void* data;
  vmaMapMemory(allocator_, stagingAllocation, &data);
  memcpy(data, indices.data(), sizeof(uint16_t) * indices.size());
  vmaUnmapMemory(allocator_, stagingAllocation);

  VkBufferCreateInfo indexBufferInfo = stagingInfo;
  indexBufferInfo.usage =
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  VmaAllocationCreateInfo indexAllocInfo = stagingAllocInfo;
  indexAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
  vmaCreateBuffer(allocator_, &indexBufferInfo, &indexAllocInfo, &indexBuffer_,
                  &indexAllocation_, nullptr);

  vk::CommandBufferAllocateInfo allocInfo{
      .commandPool = commandPool_,
      .level = vk::CommandBufferLevel::ePrimary,
      .commandBufferCount = 1};
  auto transferCmdBuf =
      std::move(vk::raii::CommandBuffers(device_, allocInfo).front());
  transferCmdBuf.begin(
      {.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
  transferCmdBuf.copyBuffer(
      stagingBuffer, indexBuffer_,
      vk::BufferCopy{.size = sizeof(uint16_t) * indices.size()});
  transferCmdBuf.end();

  vk::SubmitInfo submitInfo{.commandBufferCount = 1,
                            .pCommandBuffers = &*transferCmdBuf};
  graphicsQueue_.submit(submitInfo);

  graphicsQueue_.waitIdle();
  vmaDestroyBuffer(allocator_, stagingBuffer, stagingAllocation);
}

}  // namespace luanaut

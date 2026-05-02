#pragma once
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <glm/glm.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace luanaut {

struct Vertex {
  glm::vec3 pos;
  glm::vec3 normal;

  static auto GetBindingDescription() -> vk::VertexInputBindingDescription {
    return {.binding = 0,
            .stride = sizeof(Vertex),
            .inputRate = vk::VertexInputRate::eVertex};
  }

  static auto GetAttributeDescriptions()
      -> std::array<vk::VertexInputAttributeDescription, 2> {
    return {{{.location = 0,
              .binding = 0,
              .format = vk::Format::eR32G32B32Sfloat,
              .offset = offsetof(Vertex, pos)},
             {.location = 1,
              .binding = 0,
              .format = vk::Format::eR32G32B32Sfloat,
              .offset = offsetof(Vertex, normal)}}};
  }
};

}  // namespace luanaut

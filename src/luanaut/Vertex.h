#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <glm/glm.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace luanaut {

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;

  static auto GetBindingDescription() -> vk::VertexInputBindingDescription {
    return {.binding = 0,
            .stride = sizeof(Vertex),
            .inputRate = vk::VertexInputRate::eVertex};
  }

  static auto GetAttributeDescriptions()
      -> std::array<vk::VertexInputAttributeDescription, 2> {
    return {{{.location = 0,
              .binding = 0,
              .format = vk::Format::eR32G32Sfloat,
              .offset = offsetof(Vertex, pos)},
             {.location = 1,
              .binding = 0,
              .format = vk::Format::eR32G32B32Sfloat,
              .offset = offsetof(Vertex, color)}}};
  }
};

const std::vector<Vertex> vertices = {
    {.pos = {0.0F, -0.5F}, .color = {1.0F, 0.0F, 0.0F}},
    {.pos = {0.5F, 0.5F}, .color = {0.0F, 1.0F, 0.0F}},
    {.pos = {-0.5F, 0.5F}, .color = {0.0F, 0.0F, 1.0F}}};

}  // namespace luanaut

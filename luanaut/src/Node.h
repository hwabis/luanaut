#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace luanaut {

class Node {
 public:
  Node() = default;
  virtual ~Node() = default;

  Node(const Node&) = delete;
  auto operator=(const Node&) -> Node& = delete;
  Node(Node&&) = default;
  auto operator=(Node&&) -> Node& = default;

  glm::mat4x4 transform{1.0F};

  virtual auto Update() -> void;

  [[nodiscard]] auto GetChildren() const
      -> const std::vector<std::unique_ptr<Node>>&;
  auto AddChild(std::unique_ptr<Node> node) -> void;

 private:
  std::vector<std::unique_ptr<Node>> children_;
};

}  // namespace luanaut

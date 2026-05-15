#pragma once
#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>
#include <memory>
#include "DrawNode.h"

namespace luanaut {

class Node {
 public:
  Node() = default;
  virtual ~Node() = default;

  Node(const Node&) = delete;
  auto operator=(const Node&) -> Node& = delete;
  Node(Node&&) = default;
  auto operator=(Node&&) -> Node& = default;

  glm::mat4x4 Transform{1.0F};

  auto Expire() -> void;
  [[nodiscard]] auto GetParent() const -> Node*;
  [[nodiscard]] auto GetWorldTransform() const -> glm::mat4x4;

  auto UpdateSubTree() -> void;
  auto DrawSubTree(std::vector<DrawNode>& out) -> void;
  auto HandleEventSubTree(const SDL_Event& event) -> bool;

  [[nodiscard]] auto GetChildren() const
      -> const std::vector<std::unique_ptr<Node>>&;
  auto AddChild(std::unique_ptr<Node> node) -> void;

 protected:
  virtual auto Update() -> void;
  virtual auto Draw(std::vector<DrawNode>& out) -> void;
  virtual auto HandleEvent(const SDL_Event& event) -> bool;

 private:
  glm::mat4x4 worldTransform_{1.0F};
  Node* parent_ = nullptr;
  std::vector<std::unique_ptr<Node>> children_;
  bool isAlive_ = true;
};

}  // namespace luanaut

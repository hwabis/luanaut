#include "Node.h"

namespace luanaut {

auto Node::Update() -> void {}

auto Node::GetChildren() const -> const std::vector<std::unique_ptr<Node>>& {
  return children_;
}

auto Node::AddChild(std::unique_ptr<Node> node) -> void {
  children_.push_back(std::move(node));
}

}  // namespace luanaut

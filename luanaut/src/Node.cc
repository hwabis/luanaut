#include "Node.h"

namespace luanaut {

auto Node::UpdateSubTree() -> void {
  Update();

  for (auto& child : children_) {
    child->UpdateSubTree();
  }
}

auto Node::GetChildren() const -> const std::vector<std::unique_ptr<Node>>& {
  return children_;
}

auto Node::AddChild(std::unique_ptr<Node> node) -> void {
  children_.push_back(std::move(node));
}

auto Node::Update() -> void {}

}  // namespace luanaut

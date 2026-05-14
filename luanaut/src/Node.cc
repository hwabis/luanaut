#include "Node.h"
#include <algorithm>

namespace luanaut {

auto Node::UpdateSubTree() -> void {
  Update();

  for (auto& child : children_) {
    child->UpdateSubTree();
  }
}

auto Node::HandleEventSubTree(const SDL_Event& event) -> bool {
  if (HandleEvent(event)) {
    return true;
  }

  bool eventHandled = std::ranges::any_of(children_, [&event](auto& child) {
    return child->HandleEventSubTree(event);
  });

  return eventHandled;
}

auto Node::GetChildren() const -> const std::vector<std::unique_ptr<Node>>& {
  return children_;
}

auto Node::AddChild(std::unique_ptr<Node> node) -> void {
  children_.push_back(std::move(node));
}

auto Node::Update() -> void {}

auto Node::HandleEvent(const SDL_Event& /*event*/) -> bool {
  return false;
}

}  // namespace luanaut

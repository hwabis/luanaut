#include "Node.h"

namespace luanaut {

auto Node::UpdateSubTree() -> void {
  Update();

  for (auto& child : children_) {
    child->UpdateSubTree();
  }
}

auto Node::DrawSubTree(std::vector<DrawNode>& out) -> void {
  Draw(out);

  for (auto& child : children_) {
    child->DrawSubTree(out);
  }
}

auto Node::HandleEventSubTree(const SDL_Event& event) -> bool {
  for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
    if ((*it)->HandleEventSubTree(event)) {
      return true;
    }
  }

  return HandleEvent(event);
}

auto Node::GetChildren() const -> const std::vector<std::unique_ptr<Node>>& {
  return children_;
}

auto Node::AddChild(std::unique_ptr<Node> node) -> void {
  children_.push_back(std::move(node));
}

auto Node::Update() -> void {}

auto Node::Draw(std::vector<DrawNode>& /*out*/) -> void {}

auto Node::HandleEvent(const SDL_Event& /*event*/) -> bool {
  return false;
}

}  // namespace luanaut

#include "lneng/Node.h"

namespace lneng {

auto Node::Destroy() -> void {
  isAlive_ = false;
}

auto Node::GetNow() -> std::chrono::steady_clock::time_point {
  if (clock_ == nullptr) {
    // todo support pre-AddChild
    throw std::runtime_error(
        "Node::GetNow called before node was added to the scene tree "
        "(clock not yet wired). Call AddChild before tweening/scheduling.");
  }

  return clock_->now;
}

auto Node::GetTransform() -> Transform& {
  return transform_;
}

auto Node::GetAlpha() -> float& {
  return alpha_;
}

auto Node::GetParent() const -> Node* {
  return parent_;
}

auto Node::GetWorldTransform() const -> glm::mat4x4 {
  return worldTransform_;
}

auto Node::UpdateSubTree() -> void {
  worldTransform_ = parent_ == nullptr
                        ? transform_.ToMatrix()
                        : parent_->worldTransform_ * transform_.ToMatrix();

  UpdateTweens(clock_->now);

  Update();
  std::erase_if(children_, [](const auto& child) { return !child->isAlive_; });

  for (auto& child : children_) {
    child->UpdateSubTree();
  }
}

auto Node::DrawSubTree(SceneInfo& out) -> void {
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
  node->parent_ = this;
  auto childDeps = std::make_unique<DependencyContainer>();
  childDeps->parent = deps_.get();
  node->deps_ = std::move(childDeps);
  node->clock_ = node->deps_->Resolve<Clock>();

  node->Load();

  children_.push_back(std::move(node));
}

auto Node::ClearChildren() -> void {
  for (auto& child : children_) {
    child->Destroy();
  }
}

auto Node::IsAlive() const -> bool {
  return isAlive_;
}

auto Node::Load() -> void {}

auto Node::Update() -> void {}

auto Node::Draw(SceneInfo& /*out*/) -> void {}

auto Node::HandleEvent(const SDL_Event& /*event*/) -> bool {
  return false;
}

}  // namespace lneng

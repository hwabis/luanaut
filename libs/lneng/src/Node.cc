#include "lneng/Node.h"

namespace lneng {

auto Node::Destroy() -> void {
  isAlive_ = false;
}

auto Node::GetParent() const -> Node* {
  return parent_;
}

auto Node::GetWorldTransform() const -> glm::mat4x4 {
  return worldTransform_;
}

auto Node::UpdateSubTree() -> void {
  worldTransform_ = parent_ == nullptr
                        ? transform.ToMatrix()
                        : parent_->worldTransform_ * transform.ToMatrix();

  auto now = clock_->now;
  for (auto& tween : tweens_) {
    if (tween->IsActive(now) || tween->IsComplete(now)) {
      tween->Apply(now);
    }
  }
  std::erase_if(tweens_,
                [now](const auto& tween) { return tween->IsComplete(now); });

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
  node->clock_ = clock_;

  auto childDeps = std::make_unique<DependencyContainer>();
  childDeps->parent = deps_.get();
  node->deps_ = std::move(childDeps);

  node->Load();

  children_.push_back(std::move(node));
}

auto Node::IsAlive() const -> bool {
  return isAlive_;
}

auto Node::Move(std::chrono::steady_clock::time_point startTime,
                std::chrono::steady_clock::time_point endTime,
                glm::vec3 startVal,
                glm::vec3 endVal) -> void {
  tweens_.push_back(std::make_unique<Vec3Tween>(
      startTime, endTime, startVal, endVal,
      [this](glm::vec3 current) { transform.position = current; }));
}

auto Node::Scale(std::chrono::steady_clock::time_point startTime,
                 std::chrono::steady_clock::time_point endTime,
                 glm::vec3 startVal,
                 glm::vec3 endVal) -> void {
  tweens_.push_back(std::make_unique<Vec3Tween>(
      startTime, endTime, startVal, endVal,
      [this](glm::vec3 current) { transform.scale = current; }));
}

auto Node::Rotate(std::chrono::steady_clock::time_point startTime,
                  std::chrono::steady_clock::time_point endTime,
                  glm::quat startVal,
                  glm::quat endVal) -> void {
  tweens_.push_back(std::make_unique<QuatTween>(
      startTime, endTime, startVal, endVal,
      [this](glm::quat current) { transform.rotation = current; }));
}

auto Node::Load() -> void {}

auto Node::Update() -> void {}

auto Node::Draw(SceneInfo& /*out*/) -> void {}

auto Node::HandleEvent(const SDL_Event& /*event*/) -> bool {
  return false;
}

}  // namespace lneng

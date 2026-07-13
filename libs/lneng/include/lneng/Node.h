#pragma once
#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Clock.h"
#include "DependencyContainer.h"
#include "SceneInfo.h"
#include "Transform.h"
#include "Tweenable.h"

namespace lneng {

using namespace std::chrono_literals;

class Node : public Tweenable {
 public:
  Node() = default;
  virtual ~Node() = default;

  Node(const Node&) = delete;
  auto operator=(const Node&) -> Node& = delete;
  Node(Node&&) = delete;
  auto operator=(Node&&) -> Node& = delete;

  [[nodiscard]] auto GetNow() -> std::chrono::steady_clock::time_point override;
  [[nodiscard]] auto GetTransform() -> Transform& override;
  [[nodiscard]] auto GetAlpha() -> float& override;

  [[nodiscard]] auto GetParent() const -> Node*;
  [[nodiscard]] auto GetWorldTransform() const -> glm::mat4x4;

  [[nodiscard]] auto GetChildren() const
      -> const std::vector<std::unique_ptr<Node>>&;
  auto AddChild(std::unique_ptr<Node> node) -> void;
  auto ClearChildren() -> void;

  auto Destroy() -> void;
  [[nodiscard]] auto IsAlive() const -> bool;

 protected:
  auto UpdateSubTree() -> void;
  auto DrawSubTree(SceneInfo& out) -> void;
  auto HandleEventSubTree(const SDL_Event& event) -> bool;

  virtual auto Load() -> void;
  virtual auto Update() -> void;
  virtual auto Draw(SceneInfo& out) -> void;
  virtual auto HandleEvent(const SDL_Event& event) -> bool;

  std::unique_ptr<DependencyContainer> deps_ =
      std::make_unique<DependencyContainer>();

  // Member for performance reasons. Otherwise this would only live in deps
  Clock* clock_ = nullptr;

 private:
  Transform transform_;
  glm::mat4x4 worldTransform_{1.0F};
  Node* parent_ = nullptr;
  std::vector<std::unique_ptr<Node>> children_;
  bool isAlive_ = true;

  // todo right now fadeTo on a model does nothing.
  // only works on fullscreen node
  float alpha_{};
};

}  // namespace lneng

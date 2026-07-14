#include "lneng/Node.h"

#include <gtest/gtest.h>

namespace lneng {
namespace {

// NOLINTBEGIN(readability-magic-numbers)

class TestRoot : public Node {
 public:
  TestRoot() {
    clock_ = &clock;
    deps_->Cache(&clock);
  }

  using Node::UpdateSubTree;
  void Advance(std::chrono::milliseconds duration) { clock.now += duration; }

 private:
  Clock clock;
};

TEST(TweenTest, ZeroDurationSnapsToTarget) {
  TestRoot root;

  auto child = std::make_unique<Node>();
  auto* ptr = child.get();
  root.AddChild(std::move(child));

  ptr->MoveTo({5, 0, 0});
  root.UpdateSubTree();

  EXPECT_FLOAT_EQ(ptr->GetTransform().position.x, 5.0F);
}

TEST(DestroyTest, SweepClearsChildren) {
  TestRoot root;
  EXPECT_TRUE(root.IsAlive());

  constexpr int numChildren = 5;

  for (int i = 0; i < numChildren; ++i) {
    auto child = std::make_unique<Node>();
    root.AddChild(std::move(child));
  }

  for (const auto& child : root.GetChildren()) {
    EXPECT_TRUE(child->IsAlive());
  }

  root.ClearChildren();

  EXPECT_EQ(root.GetChildren().size(), numChildren);
  for (const auto& child : root.GetChildren()) {
    EXPECT_FALSE(child->IsAlive());
  }

  root.UpdateSubTree();
  EXPECT_EQ(root.GetChildren().size(), 0);

  root.Destroy();
  EXPECT_FALSE(root.IsAlive());
}

TEST(DestroyTest, DeadNodeDoesNotTick) {
  TestRoot root;
  auto child = std::make_unique<Node>();
  auto* ptr = child.get();
  root.AddChild(std::move(child));

  bool fired = false;
  ptr->ScheduleTask([&fired]() { fired = true; }, 100ms);

  ptr->Destroy();
  root.Advance(200ms);
  root.UpdateSubTree();

  EXPECT_FALSE(fired);
}

TEST(DestroyTest, SelfDestroyingActionDoesNotCrash) {
  TestRoot root;
  auto child = std::make_unique<Node>();
  auto* ptr = child.get();
  root.AddChild(std::move(child));
  ptr->ScheduleTask([ptr]() { ptr->Destroy(); }, 50ms);
  root.Advance(100ms);
  root.UpdateSubTree();  // fires the self-destroy — must not crash
  root.UpdateSubTree();  // sweep
  EXPECT_EQ(root.GetChildren().size(), 0);
}

TEST(SchedulerTest, ScheduledAddInsertsChild) {
  TestRoot root;
  auto container = std::make_unique<Node>();
  auto* containerPtr = container.get();
  root.AddChild(std::move(container));

  containerPtr->ScheduleTask(
      [containerPtr] { containerPtr->AddChild(std::make_unique<Node>()); },
      100ms);

  EXPECT_EQ(containerPtr->GetChildren().size(), 0);
  root.Advance(150ms);
  root.UpdateSubTree();
  EXPECT_EQ(containerPtr->GetChildren().size(), 1);
}

TEST(SchedulerTest, ScheduledDestroyRemovesChild) {
  TestRoot root;
  auto child = std::make_unique<Node>();
  auto* childPtr = child.get();
  root.AddChild(std::move(child));

  childPtr->ScheduleTask([childPtr] { childPtr->Destroy(); }, 100ms);

  EXPECT_EQ(root.GetChildren().size(), 1);
  root.Advance(150ms);
  root.UpdateSubTree();
  root.UpdateSubTree();  // sweep pass
  EXPECT_EQ(root.GetChildren().size(), 0);
}

// NOLINTEND(readability-magic-numbers)

}  // namespace
}  // namespace lneng

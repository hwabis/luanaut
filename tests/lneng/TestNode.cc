#include "lneng/Node.h"

#include <gtest/gtest.h>

namespace lneng {

TEST(NodeTest, DefaultConstructedNodeIsAlive) {
  Node node;
  EXPECT_TRUE(node.IsAlive());
}

TEST(NodeTest, DefaultConstructedNodeHasNoParent) {
  Node node;
  EXPECT_EQ(node.GetParent(), nullptr);
}

}  // namespace lneng

//
// ツリーのテスト
//

#include <gtest/gtest.h>

#include "collection2/tree.hpp"

using namespace collection2;

TEST(TreeTest, testBasicOperation) {
    TreeNode<int> nodePool[32];
    Tree<int> tree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));

    // ルートノードも値を持てる
    tree.getRootNode()->element = 123;

    // 左右に値を追加する
    TreeNode<int>* leftAdded = nullptr;
    EXPECT_EQ(tree.appendChild(tree.getRootNode(), 456, TreeNodeSide::Left, &leftAdded), OperationResult::Success);
    EXPECT_NE(leftAdded, nullptr);
    EXPECT_EQ(leftAdded->element, 456);

    TreeNode<int>* rightAdded = nullptr;
    EXPECT_EQ(tree.appendChild(tree.getRootNode(), 789, TreeNodeSide::Right, &rightAdded), OperationResult::Success);
    EXPECT_NE(rightAdded, nullptr);
    EXPECT_EQ(rightAdded->element, 789);
}

TEST(TreeTest, testDuplicate) {
    TreeNode<int> nodePool[32];
    Tree<int> tree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));

    // 同じところに二重に追加することはできない
    EXPECT_EQ(tree.appendChild(tree.getRootNode(), 123, TreeNodeSide::Left), OperationResult::Success);
    EXPECT_EQ(tree.appendChild(tree.getRootNode(), 456, TreeNodeSide::Left), OperationResult::Overflow);
    EXPECT_EQ(tree.getRootNode()->lhs->element, 123);

    EXPECT_EQ(tree.appendChild(tree.getRootNode(), 789, TreeNodeSide::Right), OperationResult::Success);
    EXPECT_EQ(tree.getRootNode()->rhs->element, 789);
}

TEST(TreeTest, testRemove) {
    TreeNode<int> nodePool[32];
    Tree<int> tree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));

    /// こんな感じの木を構成する
    ///
    ///    0
    ///   /  \
    /// 123  456
    ///      / \
    ///    999 789
    tree.appendChild(tree.getRootNode(), 123, TreeNodeSide::Left);
    TreeNode<int>* node = nullptr;
    tree.appendChild(tree.getRootNode(), 456, TreeNodeSide::Right, &node);
    tree.appendChild(node, 789, TreeNodeSide::Right);
    tree.appendChild(node, 999, TreeNodeSide::Left);
    EXPECT_EQ(tree.getRootNode()->lhs->element, 123);
    EXPECT_EQ(tree.getRootNode()->rhs->element, 456);
    EXPECT_EQ(tree.getRootNode()->rhs->rhs->element, 789);
    EXPECT_EQ(tree.getRootNode()->rhs->lhs->element, 999);

    // ルートノード右側のノードが持つ子を消す
    tree.removeChild(tree.getRootNode()->rhs);

    // 狙ったところだけ消えてる?
    EXPECT_EQ(tree.getRootNode()->lhs->element, 123);
    EXPECT_NE(tree.getRootNode()->rhs, nullptr);
    EXPECT_EQ(tree.getRootNode()->rhs->lhs, nullptr);
    EXPECT_EQ(tree.getRootNode()->rhs->rhs, nullptr);
}

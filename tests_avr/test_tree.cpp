//
// リストのテスト
//

#include "collection2/tree.hpp"
#include "testcase.hpp"

using namespace collection2;

namespace collection2tests {

TEST(testBasicOperation) {
    BeginTestcase(result);
    TreeNode<int> nodePool[32];
    Tree<int> tree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    auto* rootNode = tree.retainNode();

    // 左右に値を追加する
    TreeNode<int>* leftAdded = nullptr;
    EXPECT_EQ(tree.appendChild(rootNode, 456, TreeNodeSide::Left, &leftAdded), OperationResult::Success, result);
    EXPECT_NE(leftAdded, nullptr, result);
    EXPECT_EQ(leftAdded->element, 456, result);

    TreeNode<int>* rightAdded = nullptr;
    EXPECT_EQ(tree.appendChild(rootNode, 789, TreeNodeSide::Right, &rightAdded), OperationResult::Success, result);
    EXPECT_NE(rightAdded, nullptr, result);
    EXPECT_EQ(rightAdded->element, 789, result);
    EndTestcase(result);
}

TEST(testDuplicate) {
    BeginTestcase(result);
    TreeNode<int> nodePool[32];
    Tree<int> tree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    auto* rootNode = tree.retainNode();

    // 同じところに二重に追加することはできない
    EXPECT_EQ(tree.appendChild(rootNode, 123, TreeNodeSide::Left), OperationResult::Success, result);
    EXPECT_EQ(tree.appendChild(rootNode, 456, TreeNodeSide::Left), OperationResult::Overflow, result);
    EXPECT_EQ(rootNode->lhs->element, 123, result);

    EXPECT_EQ(tree.appendChild(rootNode, 789, TreeNodeSide::Right), OperationResult::Success, result);
    EXPECT_EQ(rootNode->rhs->element, 789, result);
    EndTestcase(result);
}

TEST(testRemove) {
    BeginTestcase(result);
    TreeNode<int> nodePool[32];
    Tree<int> tree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    auto* rootNode = tree.retainNode();

    /// こんな感じの木を構成する
    ///
    ///     0
    ///    / |
    /// 123  456
    ///     / |
    ///  999  789
    tree.appendChild(rootNode, 123, TreeNodeSide::Left);
    TreeNode<int>* node = nullptr;
    tree.appendChild(rootNode, 456, TreeNodeSide::Right, &node);
    tree.appendChild(node, 789, TreeNodeSide::Right);
    tree.appendChild(node, 999, TreeNodeSide::Left);
    EXPECT_EQ(rootNode->lhs->element, 123, result);
    EXPECT_EQ(rootNode->rhs->element, 456, result);
    EXPECT_EQ(rootNode->rhs->rhs->element, 789, result);
    EXPECT_EQ(rootNode->rhs->lhs->element, 999, result);

    // ルートノード右側のノードが持つ子を消す
    tree.removeChild(rootNode->rhs);

    // 狙ったところだけ消えてる?
    EXPECT_EQ(rootNode->lhs->element, 123, result);
    EXPECT_NE(rootNode->rhs, nullptr, result);
    EXPECT_EQ(rootNode->rhs->lhs, nullptr, result);
    EXPECT_EQ(rootNode->rhs->rhs, nullptr, result);
    EndTestcase(result);
}

const TestFunction tests[] = {
    testBasicOperation,
    testDuplicate,
    testRemove,
};

const size_t testCount = sizeof(tests) / sizeof(TestFunction);

}  // namespace collection2tests

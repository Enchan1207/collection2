//
// ツリー
//

#ifndef COLLECTION2_TREE_H
#define COLLECTION2_TREE_H

#include <stddef.h>
#include <stdint.h>

#include "common.hpp"

namespace collection2 {

/**
 * @brief ツリーの各ノードを表す構造体
 *
 * @tparam Element 要素の型
 * @tparam Size 要素数の型
 */
template <typename Element, typename Size = size_t>
struct TreeNode {
    // 有効なノードか
    bool isEnabled = false;

    // 左側子ノードへのポインタ
    TreeNode* lhs = nullptr;

    // 右側子ノードへのポインタ
    TreeNode* rhs = nullptr;

    // ノードが持つ要素
    Element element;

    /**
     * @brief リーフかどうかを調べる
     *
     * @return bool
     */
    bool isLeaf() const {
        return (lhs == nullptr) && (rhs == nullptr);
    };
};

/**
 * @brief 子ノードの方向
 */
enum class TreeNodeSide : uint8_t {
    /// @brief 左側
    Left,

    /// @brief 右側
    Right
};

/**
 * @brief ツリー
 *
 * @tparam Element 要素の型
 * @tparam Size 要素数の型
 */
template <typename Element, typename Size = size_t>
class Tree {
   private:
    /**
     * @brief 内部データ管理領域
     */
    TreeNode<Element>* const internalData;

    /**
     * @brief 内部データ長さ
     */
    Size internalDataSize;

   public:
    /**
     * @brief 内部データを扱う領域とそのサイズを指定してリストを初期化
     *
     * @param data 内部データ保管用領域
     * @param dataSize 領域サイズ
     */
    Tree(TreeNode<Element, Size>* const data, const Size& dataSize);

    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    ~Tree() = default;

    /**
     * @brief ツリーノードプールを初期化する
     * @note 既存のツリーは全て削除されます。
     */
    void initializeTreeNodePool();

    /**
     * @brief 内部ノードプールから空きノードを探し、確保する
     *
     * @return TreeNode<Element, Size>* 確保できたノードのポインタ
     * @note 空きノードがない場合はnullptrが返ります。
     */
    TreeNode<Element, Size>* retainNode() const;

    /**
     * @brief 内部ノードプールから空きノードを探し、値を割り当てる
     *
     * @return TreeNode<Element, Size>* 確保できたノードのポインタ
     * @note 空きノードがない場合はnullptrが返ります。
     */
    TreeNode<Element, Size>* retainNode(const Element& element) const;

    /**
     * @brief 子ノードを生成し、既存ノードに追加する
     *
     * @param parent 追加対象の親ノード
     * @param target 追加する要素
     * @param side 親ノードのどちらに追加するか
     * @param addedNodePtr 追加した子ノードへのポインタ
     * @return OperationResult 操作結果
     *
     * @note すでに子ノードを持っているか内部データ管理領域がいっぱいの場合、この関数は何もせずに戻ります。
     */
    OperationResult appendChild(
        TreeNode<Element, Size>* parent,
        const Element& target,
        const TreeNodeSide side,
        TreeNode<Element, Size>** addedNodePtr = nullptr);

    /**
     * @brief ノードを別のノードに接続する
     *
     * @param parent 接続元のノード
     * @param node 接続先のノード
     * @param side 接続する位置
     */
    OperationResult linkNode(
        TreeNode<Element, Size>& parent,
        TreeNode<Element, Size>* node,
        const TreeNodeSide side) const;

    /**
     * @brief 子ノードを削除する
     *
     * @param target 削除対象のノード
     *
     * @note ノードがリーフでない場合、子は再帰的に削除されます。
     */
    void removeChild(TreeNode<Element, Size>* target);

    /**
     * @brief ツリーが持てるノードおよびリーフの全体長を返す
     *
     * @return Size
     */
    Size capacity() const {
        return internalDataSize;
    }
};

template <typename Element, typename Size>
Tree<Element, Size>::Tree(TreeNode<Element, Size>* const data, const Size& dataSize)
    : internalData(data), internalDataSize(dataSize) {
    initializeTreeNodePool();
};

template <typename Element, typename Size>
inline void Tree<Element, Size>::initializeTreeNodePool() {
    for (Size i = 0; i < internalDataSize; i++) {
        internalData[i].isEnabled = false;
    }
}

template <typename Element, typename Size>
inline TreeNode<Element, Size>* Tree<Element, Size>::retainNode() const {
    for (Size i = 0; i < internalDataSize; i++) {
        if (internalData[i].isEnabled) {
            continue;
        }

        // 見つかったらノードを初期化して返す
        internalData[i].isEnabled = true;
        internalData[i].lhs = nullptr;
        internalData[i].rhs = nullptr;
        return &(internalData[i]);
    }
    return nullptr;
}

template <typename Element, typename Size>
inline TreeNode<Element, Size>* collection2::Tree<Element, Size>::retainNode(const Element& element) const {
    // ノードを確保
    auto* node = retainNode();
    if (node == nullptr) {
        return nullptr;
    }

    // 値を設定して返す
    node->element = element;
    return node;
}

template <typename Element, typename Size>
inline OperationResult collection2::Tree<Element, Size>::appendChild(
    TreeNode<Element, Size>* parent,
    const Element& target,
    const TreeNodeSide side,
    TreeNode<Element, Size>** addedNodePtr) {
    // 親ノードがnullであってはならない(単純なノードの確保はretainNodeを使う)
    if (parent == nullptr) {
        return OperationResult::Empty;
    }

    // 新しいノードをもらってくる
    auto* newNode = retainNode();
    if (newNode == nullptr) {
        if (addedNodePtr != nullptr) {
            *addedNodePtr = nullptr;
        }
        return OperationResult::Overflow;
    }

    // 値をセット
    newNode->element = target;

    // 新規生成したノードへのポインタを渡す
    if (addedNodePtr != nullptr) {
        *addedNodePtr = newNode;
    }

    // 親ノードの追加したい方に追加するノードを接続する
    return linkNode(*parent, newNode, side);
}

template <typename Element, typename Size>
inline OperationResult collection2::Tree<Element, Size>::linkNode(TreeNode<Element, Size>& parent, TreeNode<Element, Size>* node, const TreeNodeSide side) const {
    if (node == nullptr) {
        return OperationResult::Empty;
    }

    // sideで指定された方にnodeを繋ぐ すでにある場合は書き換えない
    if (side == TreeNodeSide::Left) {
        if (parent.lhs != nullptr) {
            return OperationResult::Overflow;
        }
        parent.lhs = node;
    }
    if (side == TreeNodeSide::Right) {
        if (parent.rhs != nullptr) {
            return OperationResult::Overflow;
        }
        parent.rhs = node;
    }
    return OperationResult::Success;
}

template <typename Element, typename Size>
inline void collection2::Tree<Element, Size>::removeChild(TreeNode<Element, Size>* target) {
    // リーフならデアクティベートして終わり
    if (target->isLeaf()) {
        target->isEnabled = false;
        return;
    }

    // そうでなければ再帰的に削除
    if (target->lhs != nullptr) {
        removeChild(target->lhs);
        target->lhs = nullptr;
    }
    if (target->rhs != nullptr) {
        removeChild(target->rhs);
        target->rhs = nullptr;
    }
}

}  // namespace collection2

#endif

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
    bool isLeaf() {
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

    /**
     * @brief ルートノード
     */
    TreeNode<Element>* rootNode;

    /**
     * @brief 新しいノードへのポインタを返す
     *
     * @return Node<Element>* データを追加できる位置のポインタ。内部データ領域がいっぱいの場合はnullptrが返ります。
     */
    TreeNode<Element, Size>* getNewNode() const;

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
     * @brief ルートノードへの参照を返す
     *
     * @return TreeNode<Element, Size>*
     */
    TreeNode<Element, Size>* getRootNode() const {
        return rootNode;
    }

    /**
     * @brief 子ノードを生成・追加する
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
    : internalData(data), internalDataSize(dataSize), rootNode(internalData) {
    // ルートノードを初期化しておく
    rootNode->isEnabled = true;
};

template <typename Element, typename Size>
inline OperationResult collection2::Tree<Element, Size>::appendChild(
    TreeNode<Element, Size>* parent,
    const Element& target,
    const TreeNodeSide side,
    TreeNode<Element, Size>** addedNodePtr) {
    // parentがnullであってはならない
    if (parent == nullptr) {
        return OperationResult::Empty;
    }

    // 新しいノードをもらってくる
    auto* newNode = getNewNode();
    if (newNode == nullptr) {
        if (addedNodePtr != nullptr) {
            *addedNodePtr = nullptr;
        }
        return OperationResult::Overflow;
    }

    // 追加したい方に値があるなら戻る
    auto** checkside = &((side == TreeNodeSide::Left) ? parent->lhs : parent->rhs);
    if (*checkside != nullptr) {
        return OperationResult::Overflow;
    }

    // 値をセット
    newNode->element = target;
    *checkside = newNode;

    // 新規生成したノードへのポインタを渡す
    if (addedNodePtr != nullptr) {
        *addedNodePtr = newNode;
    }
    return OperationResult::Success;
}

template <typename Element, typename Size>
inline void collection2::Tree<Element, Size>::removeChild(TreeNode<Element, Size>* target) {
    // ルートノードは削除できない
    if (target == rootNode) {
        return;
    }

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

template <typename Element, typename Size>
inline TreeNode<Element, Size>* Tree<Element, Size>::getNewNode() const {
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

}  // namespace collection2

#endif

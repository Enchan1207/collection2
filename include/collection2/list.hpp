//
// リスト
//

#ifndef _COLLECTION2_LIST_H_
#define _COLLECTION2_LIST_H_

#include <stddef.h>

#include "common.hpp"

namespace collection2 {

/**
 * @brief リストの一要素を表す構造体
 *
 * @tparam Element
 * @tparam Size
 */
template <typename Element, typename Size = size_t>
struct Node {
    // 有効なノードか
    bool isEnabled = false;

    // 次の要素へのポインタ
    Node* next = nullptr;

    // 前の要素へのポインタ
    Node* previous = nullptr;

    // ノードが持つ要素
    Element element;
};

/**
 * @brief リスト
 *
 * @tparam Element 扱う要素の型
 */
template <typename Element, typename Size = size_t>
class List {
   private:
    /**
     * @brief 内部データ管理領域
     */
    Node<Element>* const internalData;

    /**
     * @brief 内部データ長さ
     */
    Size internalDataSize;

    /**
     * @brief リスト先頭
     */
    Node<Element, Size>* headPtr = nullptr;

    /**
     * @brief リスト末尾
     */
    Node<Element, Size>* tailPtr = nullptr;

    /**
     * @brief 現在リスト内に存在するデータ数
     */
    Size count = 0;

    /**
     * @brief 新しいノードへのポインタを返す
     *
     * @return Node<Element>* データを追加できる位置のポインタ。内部データ領域がいっぱいの場合はnullptrが返ります。
     */
    Node<Element, Size>* getNewNode() const;

   public:
    /**
     * @brief 内部データを扱う領域とそのサイズを指定してリストを初期化
     *
     * @param data 内部データ保管用領域
     * @param dataSize 領域サイズ
     */
    List(Node<Element, Size>* const data, const Size& dataSize);

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    ~List() = default;

    /**
     * @brief リストの末尾にデータを追加
     *
     * @param element 追加するデータ
     * @return OperationResult 操作結果
     */
    OperationResult append(const Element& element);

    /**
     * @brief リスト内の任意の位置にデータを追加
     *
     * @param index 追加位置
     * @param element 追加するデータ
     * @return OperationResult 操作結果
     */
    OperationResult insert(const Size& index, const Element& element);

    /**
     * @brief リスト末尾のデータを削除し、取り出す
     *
     * @param element 削除したデータの格納先
     * @return OperationResult 操作結果
     */
    OperationResult pop(Element* const element);

    /**
     * @brief リスト内の任意の位置にあるデータを削除し、取り出す
     *
     * @param index 削除位置
     * @param element 削除したデータの格納先
     * @return OperationResult 操作結果
     */
    OperationResult remove(const Size& index, Element* const element);

    /**
     * @brief リスト内の要素を参照する
     *
     * @param index 参照位置
     * @return Element* 要素へのポインタ
     *
     * @note 範囲外のindexを指定した場合はnullptrが返ります。
     */
    Element* get(const Size& index);

    /**
     * @brief リスト先頭へのポインタを取得
     *
     * @return Element* リスト先頭へのポインタ
     */
    Node<Element>* head() const {
        return headPtr;
    }

    /**
     * @brief リスト末尾へのポインタを取得
     *
     * @return Element* リスト末尾へのポインタ
     */
    Node<Element>* tail() const {
        return tailPtr;
    }

    /**
     * @brief リストの全体長を返す
     *
     * @return buffer_size_t リスト長
     */
    Size capacity() const {
        return internalDataSize;
    }

    /**
     * @brief 現在リスト内にあるデータ数を返す
     *
     * @return buffer_size_t リスト内に存在するデータの数
     */
    Size amount() const {
        return count;
    }
};

template <typename Element, typename Size>
List<Element, Size>::List(Node<Element, Size>* const data, const Size& dataSize) : internalData(data), internalDataSize(dataSize){};

template <typename Element, typename Size>
Node<Element, Size>* List<Element, Size>::getNewNode() const {
    for (Size i = 0; i < internalDataSize; i++) {
        if (internalData[i].isEnabled) {
            continue;
        }

        // 見つかったらノードを初期化して返す
        internalData[i].isEnabled = true;
        internalData[i].next = nullptr;
        internalData[i].previous = nullptr;
        return &(internalData[i]);
    }
    return nullptr;
}

template <typename Element, typename Size>
OperationResult List<Element, Size>::append(const Element& element) {
    // 新しいノードを取得し、値を設定
    auto* newNode = getNewNode();
    if (newNode == nullptr) {
        return OperationResult::Overflow;
    }
    newNode->element = element;

    // 先頭の値がないならそこにセットして戻る
    if (headPtr == nullptr) {
        headPtr = newNode;
        tailPtr = newNode;
        count++;
        return OperationResult::Success;
    }

    // tailに接続
    tailPtr->next = newNode;
    newNode->previous = tailPtr;
    tailPtr = newNode;
    count++;

    return OperationResult::Success;
}

template <typename Element, typename Size>
OperationResult List<Element, Size>::insert(const Size& index, const Element& element) {
    // 新しいノードを取得し、値を設定
    auto* newNode = getNewNode();
    if (newNode == nullptr) {
        return OperationResult::Overflow;
    }
    newNode->element = element;

    // 先頭の値がないならそこにセットして戻る
    if (headPtr == nullptr) {
        headPtr = newNode;
        tailPtr = newNode;
        count++;
        return OperationResult::Success;
    }

    // 先頭への追加の場合、現在のheadが新しいノードのnextとなる
    if (index == 0) {
        newNode->next = headPtr;
        headPtr = newNode;
        count++;
        return OperationResult::Success;
    }

    // 先頭から最大index回nextを辿り、追加位置の直前のノードを取得
    auto* previousNode = headPtr;
    Size currentIndex = 0;
    while (previousNode->next != nullptr && currentIndex < index - 1) {
        previousNode = previousNode->next;
        currentIndex++;
    }

    // 接続
    if (previousNode->next == nullptr) {
        // 前のノードの次がnull -> リスト終端への追加
        tailPtr = newNode;
    } else {
        // 前のノードの次が存在 -> リスト途中への追加
        previousNode->next->previous = newNode;
    }
    newNode->next = previousNode->next;

    newNode->previous = previousNode;
    previousNode->next = newNode;

    count++;

    return OperationResult::Success;
}

template <typename Element, typename Size>
OperationResult List<Element, Size>::pop(Element* const element) {
    if (tailPtr == nullptr) {
        return OperationResult::Empty;
    }

    // 対象ノードはtailから参照できる
    auto* targetNode = tailPtr;

    // tailの位置に格納されている情報を渡す
    if (element != nullptr) {
        *element = targetNode->element;
    }

    // tailを一つ戻す
    tailPtr = targetNode->previous;
    if (tailPtr == nullptr) {
        // tailがnullになった=リストが空になった
        headPtr = nullptr;
    }

    // ノードを無効化する
    targetNode->isEnabled = false;
    targetNode->next = nullptr;
    targetNode->previous = nullptr;

    count--;

    return OperationResult::Success;
}

template <typename Element, typename Size>
OperationResult List<Element, Size>::remove(const Size& index, Element* const element) {
    if (headPtr == nullptr) {
        return OperationResult::Empty;
    }

    // 先頭のノードを削除する場合
    if (index == 0) {
        // 先頭を一つ先にずらす
        auto* targetNode = headPtr;
        headPtr = targetNode->next;
        if (headPtr == nullptr) {
            // headがnullになった=リストが空になった
            tailPtr = nullptr;
        }

        // ノードに格納されている情報を渡す
        if (element != nullptr) {
            *element = targetNode->element;
        }

        // 要素を無効化
        targetNode->isEnabled = false;
        targetNode->next = nullptr;
        targetNode->previous = nullptr;

        count--;

        return OperationResult::Success;
    }

    // 先頭からindex回、最大で配列の末尾までnextを辿り、削除位置のノードを取得
    auto* targetNode = headPtr;
    Size currentIndex = 0;
    while (targetNode->next != nullptr && currentIndex < index) {
        targetNode = targetNode->next;
        currentIndex++;
    }

    // ノードに格納されている情報を渡す
    if (element != nullptr) {
        *element = targetNode->element;
    }

    // 前後を再接続
    if (targetNode->next == nullptr) {
        // 前のノードの次がnull -> リスト終端の削除
        tailPtr = targetNode->previous;
    } else {
        // 前のノードの次が存在 -> リスト中間の削除
        targetNode->next->previous = targetNode->previous;
    }
    targetNode->previous->next = targetNode->next;

    // ノードを無効化
    targetNode->isEnabled = false;
    targetNode->next = nullptr;
    targetNode->previous = nullptr;

    count--;

    return OperationResult::Success;
}

template <typename Element, typename Size>
Element* List<Element, Size>::get(const Size& index) {
    if (headPtr == nullptr) {
        return nullptr;
    }

    // 先頭からindex回、最大で配列の末尾までnextを辿る
    auto* node = headPtr;
    Size currentIndex = 0;
    while (node->next != nullptr && currentIndex < index) {
        node = node->next;
        currentIndex++;
    }

    // インデックス範囲外
    if (currentIndex < index) {
        return nullptr;
    }

    return &(node->element);
}

}  // namespace collection2

#endif

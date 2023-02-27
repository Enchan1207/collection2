//
// リスト
//

#ifndef _COLLECTION2_LIST_H_
#define _COLLECTION2_LIST_H_

#include <climits>
#include <cstdint>

#include "common.hpp"

namespace collection2 {

/**
 * @brief リストの一要素を表す構造体
 *
 * @tparam Element 扱う要素の型
 */
template <typename Element>
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
 * @brief リストサイズを管理する変数の型
 */
using list_size_t = uint16_t;

/**
 * @brief リスト
 *
 * @tparam Element 扱う要素の型
 */
template <typename Element>
class List {
   private:
    /**
     * @brief 内部データ管理領域
     */
    Node<Element>* const internalData;

    /**
     * @brief 内部データ長さ
     */
    list_size_t internalDataSize;

    /**
     * @brief リスト先頭
     */
    Node<Element>* head = nullptr;

    /**
     * @brief リスト末尾
     */
    Node<Element>* tail = nullptr;

    /**
     * @brief 現在リスト内に存在するデータ数
     */
    list_size_t amount = 0;

    /**
     * @brief 新しいノードへのポインタを返す
     *
     * @return Node<Element>* データを追加できる位置のポインタ。内部データ領域がいっぱいの場合はnullptrが返ります。
     */
    Node<Element>* getNewNode() const;

   public:
    /**
     * @brief 内部データを扱う領域とそのサイズを指定してリストを初期化
     *
     * @param data 内部データ保管用領域
     * @param dataSize 領域サイズ
     */
    List(Node<Element>* const data, const list_size_t& dataSize);

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
    OperationResult insert(const list_size_t& index, const Element& element);

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
    OperationResult remove(const list_size_t& index, Element* const element);

    /**
     * @brief リスト内の要素を参照する
     *
     * @param index 参照位置
     * @return Element* 要素へのポインタ
     *
     * @note 範囲外のindexを指定した場合はnullptrが返ります。
     */
    Element* get(const list_size_t& index);
};

template <typename Element>
List<Element>::List(Node<Element>* const data, const list_size_t& dataSize) : internalData(data), internalDataSize(dataSize){};

template <typename Element>
Node<Element>* List<Element>::getNewNode() const {
    for (list_size_t i = 0; i < internalDataSize; i++) {
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

template <typename Element>
OperationResult List<Element>::append(const Element& element) {
    // 新しいノードを取得し、値を設定
    auto* newNode = getNewNode();
    if (newNode == nullptr) {
        return OperationResult::Overflow;
    }
    newNode->element = element;

    // 先頭の値がないならそこにセットして戻る
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
        return OperationResult::Success;
    }

    // tailに接続
    tail->next = newNode;
    newNode->previous = tail;
    tail = newNode;

    return OperationResult::Success;
}

template <typename Element>
OperationResult List<Element>::insert(const list_size_t& index, const Element& element) {
    // 新しいノードを取得し、値を設定
    auto* newNode = getNewNode();
    if (newNode == nullptr) {
        return OperationResult::Overflow;
    }
    newNode->element = element;

    // 先頭の値がないならそこにセットして戻る
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
        return OperationResult::Success;
    }

    // 先頭への追加の場合、現在のheadが新しいノードのnextとなる
    if (index == 0) {
        newNode->next = head;
        head = newNode;
        return OperationResult::Success;
    }

    // 先頭から最大index回nextを辿り、追加位置の直前のノードを取得
    auto* previousNode = head;
    list_size_t currentIndex = 0;
    while (previousNode->next != nullptr && currentIndex < index - 1) {
        previousNode = previousNode->next;
        currentIndex++;
    }

    // 接続
    if (previousNode->next == nullptr) {
        // 前のノードの次がnull -> リスト終端への追加
        tail = newNode;
    } else {
        // 前のノードの次が存在 -> リスト途中への追加
        previousNode->next->previous = newNode;
    }
    newNode->next = previousNode->next;

    newNode->previous = previousNode;
    previousNode->next = newNode;

    return OperationResult::Success;
}

template <typename Element>
OperationResult List<Element>::pop(Element* const element) {
    if (tail == nullptr) {
        return OperationResult::Empty;
    }

    // 対象ノードはtailから参照できる
    auto* targetNode = tail;

    // tailの位置に格納されている情報を渡す
    if (element != nullptr) {
        *element = targetNode->element;
    }

    // tailを一つ戻す
    tail = targetNode->previous;
    if (tail == nullptr) {
        // tailがnullになった=リストが空になった
        head = nullptr;
    }

    // ノードを無効化する
    targetNode->isEnabled = false;
    targetNode->next = nullptr;
    targetNode->previous = nullptr;

    return OperationResult::Success;
}

template <typename Element>
OperationResult List<Element>::remove(const list_size_t& index, Element* const element) {
    if (head == nullptr) {
        return OperationResult::Empty;
    }

    // 先頭のノードを削除する場合
    if (index == 0) {
        // 先頭を一つ先にずらす
        auto* targetNode = head;
        head = targetNode->next;
        if (head == nullptr) {
            // headがnullになった=リストが空になった
            tail = nullptr;
        }

        // ノードに格納されている情報を渡す
        if (element != nullptr) {
            *element = targetNode->element;
        }

        // 要素を無効化
        targetNode->isEnabled = false;
        targetNode->next = nullptr;
        targetNode->previous = nullptr;

        return OperationResult::Success;
    }

    // 先頭からindex回、最大で配列の末尾までnextを辿り、削除位置のノードを取得
    auto* targetNode = head;
    list_size_t currentIndex = 0;
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
        tail = targetNode->previous;
    } else {
        // 前のノードの次が存在 -> リスト中間の削除
        targetNode->next->previous = targetNode->previous;
    }
    targetNode->previous->next = targetNode->next;

    // ノードを無効化
    targetNode->isEnabled = false;
    targetNode->next = nullptr;
    targetNode->previous = nullptr;

    return OperationResult::Success;
}

template <typename Element>
Element* List<Element>::get(const list_size_t& index) {
    if (head == nullptr) {
        return nullptr;
    }

    // 先頭からindex回、最大で配列の末尾までnextを辿る
    auto* node = head;
    list_size_t currentIndex = 0;
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

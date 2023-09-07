//
// キュー
//

#ifndef _COLLECTION2_QUEUE_H_
#define _COLLECTION2_QUEUE_H_

#include <stddef.h>

#include "common.hpp"

namespace collection2 {

/**
 * @brief キュー
 *
 * @tparam Element
 * @tparam Size
 */
template <typename Element, typename Size = size_t>
class Queue {
   private:
    /**
     * @brief 内部データ管理領域
     */
    Element* const internalData;

    /**
     * @brief 内部データ長さ
     */
    Size internalDataSize;

    /**
     * @brief キュー先頭
     */
    Size head = 0;

    /**
     * @brief キュー末尾
     */
    Size tail = 0;

    /**
     * @brief 現在キュー内に存在するデータ数
     */
    Size count = 0;

   public:
    /**
     * @brief 内部データを扱う領域とそのサイズを指定してキューを初期化
     *
     * @param data 内部データ保管用領域
     * @param dataSize 領域サイズ
     * @note 領域サイズは2の冪乗であるべきです。それ以外の値を指定した場合、2の冪数のうち領域のサイズを下回らない最大のものが選択されます(15 -> 8, 34 -> 32).
     */
    Queue(Element* const data, const Size& dataSize);

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    ~Queue() = default;

    /**
     * @brief キューにデータを追加
     *
     * @param data 追加するデータ
     * @return OperationResult 操作結果
     */
    OperationResult enqueue(const Element& data);

    /**
     * @brief キューからデータを取り出し
     *
     * @param data 取り出したデータの格納先
     * @return OperationResult 操作結果
     */
    OperationResult dequeue(Element* const data);

    /**
     * @brief キューの全体長を返す
     *
     * @return buffer_size_t キュー長
     */
    Size capacity() const {
        return internalDataSize;
    }

    /**
     * @brief 現在キュー内にあるデータ数を返す
     *
     * @return buffer_size_t キュー内に存在するデータの数
     */
    Size amount() const {
        return count;
    }

    /**
     * @brief キューに値を追加できるか
     *
     * @return bool
     */
    bool hasSpace() const {
        return count < internalDataSize;
    }

    /**
     * @brief キューが空かどうか
     *
     * @return bool
     */
    bool isEmpty() const {
        return count == 0;
    }
};

template <typename Element, typename Size>
Queue<Element, Size>::Queue(Element* const data, const Size& dataSize) : internalData(data) {
    // ゼロ長のキューなら何もしない
    if (dataSize == 0) {
        internalDataSize = dataSize;
        return;
    }

    // 与えられたサイズを上回らない最大の2の冪数を探す
    unsigned char maxbitPos = 0;
    Size size = dataSize;
    while ((size >>= 1) != 0) {
        maxbitPos++;
    }
    internalDataSize = 1 << maxbitPos;
};

template <typename Element, typename Size>
OperationResult Queue<Element, Size>::enqueue(const Element& data) {
    // キューがいっぱいなら戻る
    if (!hasSpace()) {
        return OperationResult::Overflow;
    }

    // tailの位置にデータを書き込む
    *(internalData + tail) = data;

    tail = (tail + 1) & (internalDataSize - 1);
    count++;

    return OperationResult::Success;
}

template <typename Element, typename Size>
OperationResult Queue<Element, Size>::dequeue(Element* const data) {
    // キューが空なら戻る
    if (isEmpty()) {
        return OperationResult::Empty;
    }

    // 読み出して渡す
    *data = *(internalData + head);

    head = (head + 1) & (internalDataSize - 1);
    count--;

    return OperationResult::Success;
}

}  // namespace collection2

#endif

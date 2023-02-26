//
// キュー
//

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <climits>
#include <cstdint>

namespace collection2 {

/**
 * @brief キュー操作結果
 */
enum class QueueOperationResult {
    Success,
    Overflow,
    Empty
};

/**
 * @brief キューサイズを管理する変数の型
 */
using queue_size_t = uint16_t;

/**
 * @brief キュー
 *
 * @tparam Element 扱う要素の型
 */
template <typename Element>
class Queue {
   private:
    /**
     * @brief 内部データ管理領域
     */
    Element* const internalData;

    /**
     * @brief 内部データ長さ
     */
    queue_size_t internalDataSize;

    /**
     * @brief キュー先頭
     */
    queue_size_t head = 0;

    /**
     * @brief キュー末尾
     */
    queue_size_t tail = 0;

    /**
     * @brief 現在キュー内に存在するデータ数
     */
    queue_size_t amount = 0;

   public:
    /**
     * @brief 内部データを扱う領域とそのサイズを指定してキューを初期化
     *
     * @param data 内部データ保管用領域
     * @param dataSize 領域サイズ
     * @note 領域サイズは2の冪乗であるべきです。それ以外の値を指定した場合、2の冪数のうち領域のサイズを下回らない最大のものが選択されます(15 -> 8, 34 -> 32).
     */
    Queue(Element* const data, const queue_size_t& dataSize);

    ~Queue() = default;

    /**
     * @brief キューにデータを追加
     *
     * @param data 追加するデータ
     * @return QueueOperationResult 操作結果
     */
    QueueOperationResult enqueue(const Element& data);

    /**
     * @brief キューからデータを取り出し
     *
     * @param data 取り出したデータの格納先
     * @return QueueOperationResult 操作結果
     */
    QueueOperationResult dequeue(Element* const data);

    /**
     * @brief キューに値を追加できるか
     *
     * @return bool
     */
    bool hasSpace() const {
        return amount < internalDataSize;
    }

    /**
     * @brief キューが空かどうか
     *
     * @return bool 空ならtrue、データが存在すればfalse
     */
    bool isEmpty() const {
        return amount == 0;
    }
};

template <typename Element>
Queue<Element>::Queue(Element* const data, const queue_size_t& dataSize) : internalData(data) {
    // ゼロ長のキューなら何もしない
    if (dataSize == 0) {
        internalDataSize = dataSize;
        return;
    }

    // 与えられたサイズを上回らない最大の2の冪数を探す
    const uint8_t queueSizeBitLength = sizeof(queue_size_t) * CHAR_BIT;  // キューサイズのビット数
    uint8_t currentBitPosition = queueSizeBitLength;
    auto candidate = 0;
    do {
        candidate = static_cast<queue_size_t>(1 << currentBitPosition);
        if ((candidate & dataSize) != 0) {
            break;
        }
    } while (currentBitPosition--);
    internalDataSize = candidate;
};

template <typename Element>
QueueOperationResult Queue<Element>::enqueue(const Element& data) {
    // キューがいっぱいなら戻る
    if (!hasSpace()) {
        return QueueOperationResult::Overflow;
    }

    // tailの位置にデータを書き込む
    *(internalData + tail) = data;

    tail = (tail + 1) & (internalDataSize - 1);
    amount++;

    return QueueOperationResult::Success;
}

template <typename Element>
QueueOperationResult Queue<Element>::dequeue(Element* const data) {
    // キューが空なら戻る
    if (isEmpty()) {
        return QueueOperationResult::Empty;
    }

    // 読み出して渡す
    *data = *(internalData + head);

    head = (head + 1) & (internalDataSize - 1);
    amount--;

    return QueueOperationResult::Success;
}

}  // namespace collection2

#endif

//
// リングバッファ
//

#ifndef _COLLECTION2_BUFFER_H_
#define _COLLECTION2_BUFFER_H_

#include <limits.h>
#include <stdint.h>

#include "common.hpp"

namespace collection2 {

/**
 * @brief バッファサイズを管理する変数の型
 */
using buffer_size_t = uint16_t;

/**
 * @brief リングバッファ
 *
 * @tparam Element 扱う要素の型
 */
template <typename Element>
class Buffer {
   private:
    /**
     * @brief 内部データ管理領域
     */
    Element* const internalData;

    /**
     * @brief 内部データ長さ
     */
    buffer_size_t internalDataSize;

    /**
     * @brief バッファ先頭
     */
    buffer_size_t head = 0;

    /**
     * @brief バッファ末尾
     */
    buffer_size_t tail = 0;

    /**
     * @brief 現在バッファ内に存在するデータ数
     */
    buffer_size_t count = 0;

   public:
    /**
     * @brief 内部データを扱う領域とそのサイズを指定してバッファを初期化
     *
     * @param data 内部データ保管用領域
     * @param dataSize 領域サイズ
     * @note 領域サイズは2の冪乗であるべきです。それ以外の値を指定した場合、2の冪数のうち領域のサイズを下回らない最大のものが選択されます(15 -> 8, 34 -> 32).
     */
    Buffer(Element* const data, const buffer_size_t& dataSize);

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    ~Buffer() = default;

    /**
     * @brief バッファの末尾にデータを追加
     *
     * @param data 追加するデータ
     * @return OperationResult 操作結果
     */
    OperationResult append(const Element& data);

    /**
     * @brief バッファの先頭からデータを取り出し
     *
     * @param data 取り出したデータの格納先
     * @return OperationResult 操作結果
     */
    OperationResult pop(Element* const data);

    /**
     * @brief バッファの全体長を返す
     *
     * @return buffer_size_t バッファ長
     */
    buffer_size_t capacity() const {
        return internalDataSize;
    }

    /**
     * @brief 現在バッファ内にあるデータ数を返す
     *
     * @return buffer_size_t バッファ内に存在するデータの数
     */
    buffer_size_t amount() const {
        return count;
    }

    /**
     * @brief バッファに値を追加できるか
     *
     * @return bool
     */
    bool hasSpace() const {
        return count < internalDataSize;
    }

    /**
     * @brief バッファが空かどうか
     *
     * @return bool
     */
    bool isEmpty() const {
        return count == 0;
    }
};

template <typename Element>
Buffer<Element>::Buffer(Element* const data, const buffer_size_t& dataSize) : internalData(data) {
    // ゼロ長のバッファなら何もしない
    if (dataSize == 0) {
        internalDataSize = dataSize;
        return;
    }

    // 与えられたサイズを上回らない最大の2の冪数を探す
    const uint8_t BufferSizeBitLength = sizeof(buffer_size_t) * CHAR_BIT;  // バッファサイズのビット数
    uint8_t currentBitPosition = BufferSizeBitLength;
    buffer_size_t candidate = 0;
    do {
        candidate = static_cast<buffer_size_t>(1 << currentBitPosition);
        if ((candidate & dataSize) != 0) {
            break;
        }
    } while (currentBitPosition--);
    internalDataSize = candidate;
};

template <typename Element>
OperationResult Buffer<Element>::append(const Element& data) {
    // サイズ0のバッファに値を追加することはできない
    if (internalDataSize == 0) {
        return OperationResult::Overflow;
    }

    // バッファがいっぱいなら、要素を一つ読み捨てる
    if (!hasSpace()) {
        auto result = pop(nullptr);
    }

    // tailの位置にデータを書き込む
    *(internalData + tail) = data;

    tail = (tail + 1) & (internalDataSize - 1);
    count++;

    return OperationResult::Success;
}

template <typename Element>
OperationResult Buffer<Element>::pop(Element* const data) {
    // バッファが空なら戻る
    if (isEmpty()) {
        return OperationResult::Empty;
    }

    // 読み出して渡す。nullptrなら何もしないでheadを進める。
    if (data != nullptr) {
        *data = *(internalData + head);
    }

    head = (head + 1) & (internalDataSize - 1);
    count--;

    return OperationResult::Success;
}

}  // namespace collection2

#endif

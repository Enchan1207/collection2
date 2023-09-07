//
// スタック
//

#ifndef _COLLECTION2_STACK_H_
#define _COLLECTION2_STACK_H_

#include <stddef.h>

#include "common.hpp"

namespace collection2 {

/**
 * @brief スタック
 *
 * @tparam Element
 * @tparam Size
 */
template <typename Element, typename Size = size_t>
class Stack {
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
     * @brief スタックポインタ
     */
    Size sp = 0;

   public:
    /**
     * @brief 内部データを扱う領域とそのサイズを指定してスタックを初期化
     *
     * @param data 内部データ保管用領域
     * @param dataSize 領域サイズ
     */
    Stack(Element* const data, const Size& dataSize);

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    ~Stack() = default;

    /**
     * @brief スタックにデータを追加
     *
     * @param data 追加するデータ
     * @return OperationResult 操作結果
     */
    OperationResult push(const Element& data);

    /**
     * @brief スタックからデータを取り出し
     *
     * @param data 取り出したデータの格納先
     * @return OperationResult 操作結果
     */
    OperationResult pop(Element* const data);

    /**
     * @brief スタックの全体長を返す
     *
     * @return buffer_size_t スタック長
     */
    Size capacity() const {
        return internalDataSize;
    }

    /**
     * @brief 現在スタック内にあるデータ数を返す
     *
     * @return buffer_size_t スタック内に存在するデータの数
     */
    Size amount() const {
        return sp;
    }

    /**
     * @brief スタックに値を追加できるか
     *
     * @return bool
     */
    bool hasSpace() const {
        return sp < internalDataSize;
    }

    /**
     * @brief スタックが空かどうか
     *
     * @return bool
     */
    bool isEmpty() const {
        return sp == 0;
    }
};

template <typename Element, typename Size>
Stack<Element, Size>::Stack(Element* const data, const Size& dataSize) : internalData(data), internalDataSize(dataSize){};

template <typename Element, typename Size>
OperationResult Stack<Element, Size>::push(const Element& data) {
    // スタックがいっぱいなら戻る
    if (!hasSpace()) {
        return OperationResult::Overflow;
    }

    // spの位置にデータを書き込んで進める
    *(internalData + sp) = data;
    sp++;

    return OperationResult::Success;
}

template <typename Element, typename Size>
OperationResult Stack<Element, Size>::pop(Element* const data) {
    // スタックが空なら戻る
    if (isEmpty()) {
        return OperationResult::Empty;
    }

    // spを減らしてデータを読み出す
    sp--;
    *data = *(internalData + sp);
    return OperationResult::Success;
}

}  // namespace collection2

#endif

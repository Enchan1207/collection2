/** @file */
//
// 共通
//

#ifndef COLLECTION2_COMMON_H
#define COLLECTION2_COMMON_H

namespace collection2 {

/**
 * @brief コレクション操作結果
 */
enum class OperationResult {

    /**
     * @brief 成功
     */
    Success,

    /**
     * @brief コレクションに値を追加しようとしたが、満杯だった
     */
    Overflow,

    /**
     * @brief コレクションから値を取り出そうとしたが、何も格納されていなかった
     */
    Empty
};

}  // namespace collection2

#endif /* COLLECTION2_COMMON_H */

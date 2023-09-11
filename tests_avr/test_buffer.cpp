//
// バッファのテスト
//

#include "collection2/buffer.hpp"
#include "testcase.hpp"

using namespace collection2;

namespace collection2tests {

TEST(testInstantiate) {
    BeginTestcase(result);

    int data[10] = {0};
    Buffer<int> buffer(data, 10);

    EXPECT_EQ(buffer.capacity(), 8, result);

    buffer.append(1);
    buffer.append(2);
    buffer.append(3);
    buffer.append(4);
    buffer.append(5);

    EXPECT_EQ(buffer.amount(), 5, result);

    EndTestcase(result);
}

TEST(testOperateWithOrdinallySize) {
    BeginTestcase(result);

    // int型を扱うバッファを定義
    const int BufferLength = 8;
    int bufferData[BufferLength] = {0};
    Buffer<int> buffer(bufferData, BufferLength);

    // バッファの長さだけ値を追加
    for (int i = 0; i < BufferLength; i++) {
        EXPECT_EQ(buffer.append(i + 1), OperationResult::Success, result);
    }

    // 範囲外の追加にも成功するが、最初の要素が読み捨てられる
    EXPECT_EQ(buffer.append(9), OperationResult::Success, result);

    // 値を取り出す
    for (int i = 1; i < BufferLength + 1; i++) {
        int value = -1;
        EXPECT_EQ(buffer.pop(&value), OperationResult::Success, result);
        EXPECT_EQ(value, i + 1, result);
    }

    // 範囲外の取り出し
    int dummy = 0;
    EXPECT_EQ(buffer.pop(&dummy), OperationResult::Empty, result);

    EndTestcase(result);
}

TEST(testOperateWithExceptionalSize) {
    BeginTestcase(result);
    // 2の冪数でない場合…
    const int bufferLength = 100;
    int bufferData[bufferLength] = {0};
    Buffer<int> buffer(bufferData, bufferLength);

    // バッファの長さに最も近い2の冪数回だけ値を追加
    const int expectedBufferLength = 64;
    for (int i = 0; i < expectedBufferLength; i++) {
        EXPECT_EQ(buffer.append(i + 1), OperationResult::Success, result);
    }

    // 値を取り出す
    for (int i = 0; i < expectedBufferLength; i++) {
        int value = 0;
        EXPECT_EQ(buffer.pop(&value), OperationResult::Success, result);
        EXPECT_EQ(value, i + 1, result);
    }

    // 範囲外の取り出しとなるか?
    int dummy = 0;
    EXPECT_EQ(buffer.pop(&dummy), OperationResult::Empty, result);
    EndTestcase(result);
}

TEST(testOperateWithZeroSize) {
    BeginTestcase(result);
    // サイズ0のバッファ
    Buffer<int> buffer(nullptr, 0);

    // 値の追加・取り出しに失敗する
    int value = 1;
    EXPECT_EQ(buffer.append(value), OperationResult::Overflow, result);
    EXPECT_EQ(buffer.pop(&value), OperationResult::Empty, result);
    EndTestcase(result);
}

TEST(testOperateWithOneSize) {
    BeginTestcase(result);
    // サイズ1のバッファ
    const int bufferLength = 1;
    int bufferData[bufferLength] = {0};
    Buffer<int> buffer(bufferData, 1);

    // 値の追加・取り出しが問題なく成功するか
    EXPECT_EQ(buffer.append(1), OperationResult::Success, result);
    EXPECT_EQ(buffer.append(2), OperationResult::Success, result);

    int value = 0;
    EXPECT_EQ(buffer.pop(&value), OperationResult::Success, result);
    EXPECT_EQ(value, 2, result);
    EXPECT_EQ(buffer.pop(&value), OperationResult::Empty, result);
    EndTestcase(result);
}

const TestFunction tests[] = {
    testInstantiate,
    testOperateWithOrdinallySize,
    testOperateWithExceptionalSize,
    testOperateWithZeroSize,
    testOperateWithOneSize,
};

const size_t testCount = 5;

}  // namespace collection2tests

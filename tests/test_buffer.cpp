//
// バッファのテスト
//

#include <gtest/gtest.h>

#include "collection2/buffer.hpp"

using namespace collection2;

TEST(BufferTest, testInstantiate) {
    // プリミティブ型
    char charBufferData[8] = {0};
    Buffer<char> charBuffer(charBufferData, 8);

    // 構造体
    struct Structure {
        int integer;
        char charactor;
    };
    Structure structBufferData[8] = {0};
    Buffer<Structure> structBuffer(structBufferData, 8);
}

TEST(BufferTest, testCapacityAndAmount) {
    int data[10] = {0};
    Buffer<int> buffer(data, 10);

    EXPECT_EQ(buffer.capacity(), 8);

    buffer.append(1);
    buffer.append(2);
    buffer.append(3);
    buffer.append(4);
    buffer.append(5);

    EXPECT_EQ(buffer.amount(), 5);
}

TEST(BufferTest, testOperateWithOrdinallySize) {
    // int型を扱うバッファを定義
    const int BufferLength = 8;
    int bufferData[BufferLength] = {0};
    Buffer<int> buffer(bufferData, BufferLength);

    // バッファの長さだけ値を追加
    for (int i = 0; i < BufferLength; i++) {
        EXPECT_EQ(buffer.append(i + 1), OperationResult::Success);
    }

    // 範囲外の追加にも成功するが、最初の要素が読み捨てられる
    EXPECT_EQ(buffer.append(9), OperationResult::Success);

    // 値を取り出す
    for (int i = 1; i < BufferLength + 1; i++) {
        int value = -1;
        EXPECT_EQ(buffer.pop(&value), OperationResult::Success);
        EXPECT_EQ(value, i + 1);
    }

    // 範囲外の取り出し
    int dummy = 0;
    EXPECT_EQ(buffer.pop(&dummy), OperationResult::Empty);
}

TEST(BufferTest, testOperateWithExceptionalSize) {
    // 2の冪数でない場合…
    const int bufferLength = 100;
    int bufferData[bufferLength] = {0};
    Buffer<int> buffer(bufferData, bufferLength);

    // バッファの長さに最も近い2の冪数回だけ値を追加
    const int expectedBufferLength = 64;
    for (int i = 0; i < expectedBufferLength; i++) {
        EXPECT_EQ(buffer.append(i + 1), OperationResult::Success);
    }

    // 値を取り出す
    for (int i = 0; i < expectedBufferLength; i++) {
        int value = 0;
        EXPECT_EQ(buffer.pop(&value), OperationResult::Success);
        EXPECT_EQ(value, i + 1);
    }

    // 範囲外の取り出しとなるか?
    int dummy = 0;
    EXPECT_EQ(buffer.pop(&dummy), OperationResult::Empty);
}

TEST(BufferTest, testOperateWithZeroSize) {
    // サイズ0のバッファ
    Buffer<int> buffer(nullptr, 0);

    // 値の追加・取り出しに失敗する
    int value = 1;
    EXPECT_EQ(buffer.append(value), OperationResult::Overflow);
    EXPECT_EQ(buffer.pop(&value), OperationResult::Empty);
}

TEST(BufferTest, testOperateWithOneSize) {
    // サイズ1のバッファ
    const int bufferLength = 1;
    int bufferData[bufferLength] = {0};
    Buffer<int> buffer(bufferData, 1);

    // 値の追加・取り出しが問題なく成功するか
    EXPECT_EQ(buffer.append(1), OperationResult::Success);
    EXPECT_EQ(buffer.append(2), OperationResult::Success);

    int value = 0;
    EXPECT_EQ(buffer.pop(&value), OperationResult::Success);
    EXPECT_EQ(value, 2);
    EXPECT_EQ(buffer.pop(&value), OperationResult::Empty);
}

//
// スタックのテスト
//

#include <gtest/gtest.h>

#include "collection2/stack.hpp"

using namespace collection2;

TEST(StackTest, testInstantiate) {
    // プリミティブ型
    char charStackData[8] = {0};
    Stack<char> charStack(charStackData, 8);

    // 構造体
    struct Structure {
        int integer;
        char charactor;
    };
    Structure structStackData[8] = {0};
    Stack<Structure> structStack(structStackData, 8);
}

TEST(StackTest, testOperate) {
    // int型を扱う長さ100のスタックを定義
    const int stackLength = 100;
    int StackData[stackLength] = {0};
    Stack<int> Stack(StackData, stackLength);

    // スタックの長さだけ値を追加
    for (int i = 0; i < stackLength; i++) {
        EXPECT_EQ(Stack.push(i + 1), OperationResult::Success);
    }

    // 範囲外の追加
    EXPECT_EQ(Stack.push(0), OperationResult::Overflow);

    // 値を取り出す
    for (int i = stackLength; i > 0; i--) {
        int value = 0;
        EXPECT_EQ(Stack.pop(&value), OperationResult::Success);
        EXPECT_EQ(value, i);
    }

    // 範囲外の取り出し
    int dummy = 0;
    EXPECT_EQ(Stack.pop(&dummy), OperationResult::Empty);
}

TEST(StackTest, testOperateWithZeroSize) {
    // サイズ0のスタック
    Stack<int> Stack(nullptr, 0);

    // 値の追加・取り出しに失敗する
    int value = 1;
    EXPECT_EQ(Stack.push(value), OperationResult::Overflow);
    EXPECT_EQ(Stack.pop(&value), OperationResult::Empty);
}

TEST(StackTest, testOperateWithOneSize) {
    // サイズ1のスタック
    const int stackLength = 1;
    int StackData[stackLength] = {0};
    Stack<int> Stack(StackData, 1);

    // 値の追加・取り出しが問題なく成功するか
    int value = 1;
    EXPECT_EQ(Stack.push(value), OperationResult::Success);
    EXPECT_EQ(Stack.push(value + 1), OperationResult::Overflow);

    value = 0;
    EXPECT_EQ(Stack.pop(&value), OperationResult::Success);
    EXPECT_EQ(value, 1);
    EXPECT_EQ(Stack.pop(&value), OperationResult::Empty);
}

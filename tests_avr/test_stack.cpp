//
// スタックのテスト
//

#include "collection2/stack.hpp"
#include "testcase.hpp"

using namespace collection2;

namespace collection2tests {

TEST(testInstantiate) {
    BeginTestcase(result);
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
    EndTestcase(result);
}

// バッファ長
TEST(testCapacityAndAmount) {
    BeginTestcase(result);
    int data[10] = {0};
    Stack<int> stack(data, 10);

    EXPECT_EQ(stack.capacity(), 10, result);

    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.push(5);

    EXPECT_EQ(stack.amount(), 5, result);
    EndTestcase(result);
}

TEST(testOperate) {
    BeginTestcase(result);
    // int型を扱う長さ100のスタックを定義
    const int stackLength = 100;
    int StackData[stackLength] = {0};
    Stack<int> Stack(StackData, stackLength);

    // スタックの長さだけ値を追加
    for (int i = 0; i < stackLength; i++) {
        EXPECT_EQ(Stack.push(i + 1), OperationResult::Success, result);
    }

    // 範囲外の追加
    EXPECT_EQ(Stack.push(0), OperationResult::Overflow, result);

    // 値を取り出す
    for (int i = stackLength; i > 0; i--) {
        int value = 0;
        EXPECT_EQ(Stack.pop(&value), OperationResult::Success, result);
        EXPECT_EQ(value, i, result);
    }

    // 範囲外の取り出し
    int dummy = 0;
    EXPECT_EQ(Stack.pop(&dummy), OperationResult::Empty, result);
    EndTestcase(result);
}

TEST(testOperateWithZeroSize) {
    BeginTestcase(result);
    // サイズ0のスタック
    Stack<int> Stack(nullptr, 0);

    // 値の追加・取り出しに失敗する
    int value = 1;
    EXPECT_EQ(Stack.push(value), OperationResult::Overflow, result);
    EXPECT_EQ(Stack.pop(&value), OperationResult::Empty, result);
    EndTestcase(result);
}

TEST(testOperateWithOneSize) {
    BeginTestcase(result);
    // サイズ1のスタック
    const int stackLength = 1;
    int StackData[stackLength] = {0};
    Stack<int> Stack(StackData, 1);

    // 値の追加・取り出しが問題なく成功するか
    int value = 1;
    EXPECT_EQ(Stack.push(value), OperationResult::Success, result);
    EXPECT_EQ(Stack.push(value + 1), OperationResult::Overflow, result);

    value = 0;
    EXPECT_EQ(Stack.pop(&value), OperationResult::Success, result);
    EXPECT_EQ(value, 1, result);
    EXPECT_EQ(Stack.pop(&value), OperationResult::Empty, result);
    EndTestcase(result);
}

const TestFunction tests[] = {
    testInstantiate,
    testCapacityAndAmount,
    testOperate,
    testOperateWithZeroSize,
    testOperateWithOneSize,
};

const size_t testCount = 5;

}  // namespace collection2tests

//
// キューのテスト
//

#include "collection2/queue.hpp"
#include "testcase.hpp"

using namespace collection2;

namespace collection2tests {

TEST(testInstantiate) {
    BeginTestcase(result);
    // プリミティブ型
    char charQueueData[8] = {0};
    Queue<char> charQueue(charQueueData, 8);

    // 構造体
    struct Structure {
        int integer;
        char charactor;
    };
    Structure structQueueData[8] = {0};
    Queue<Structure> structQueue(structQueueData, 8);
    EndTestcase(result);
}

TEST(testCapacityAndAmount) {
    BeginTestcase(result);
    int data[10] = {0};
    Queue<int> queue(data, 10);

    EXPECT_EQ(queue.capacity(), 8, result);

    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    queue.enqueue(4);
    queue.enqueue(5);

    EXPECT_EQ(queue.amount(), 5, result);
    EndTestcase(result);
}

TEST(testOperateWithOrdinallySize) {
    BeginTestcase(result);
    // int型を扱う長さ256のキューを定義
    const int queueLength = 256;
    int queueData[queueLength] = {0};
    Queue<int> queue(queueData, queueLength);

    // キューの長さだけ値を追加
    for (int i = 0; i < queueLength; i++) {
        EXPECT_EQ(queue.enqueue(i + 1), OperationResult::Success, result);
    }

    // 範囲外の追加
    EXPECT_EQ(queue.enqueue(0), OperationResult::Overflow, result);

    // 値を取り出す
    for (int i = 0; i < queueLength; i++) {
        int value = 0;
        EXPECT_EQ(queue.dequeue(&value), OperationResult::Success, result);
        EXPECT_EQ(value, i + 1, result);
    }

    // 範囲外の取り出し
    int dummy = 0;
    EXPECT_EQ(queue.dequeue(&dummy), OperationResult::Empty, result);
    EndTestcase(result);
}

TEST(testOperateWithExceptionalSize) {
    BeginTestcase(result);
    // 2の冪数でない場合…
    const int queueLength = 100;
    int queueData[queueLength] = {0};
    Queue<int> queue(queueData, queueLength);

    // キューの長さに最も近い2の冪数回だけ値を追加
    const int expectedQueueLength = 64;
    for (int i = 0; i < expectedQueueLength; i++) {
        EXPECT_EQ(queue.enqueue(i + 1), OperationResult::Success, result);
    }

    // 範囲外となるか?
    EXPECT_EQ(queue.enqueue(0), OperationResult::Overflow, result);

    // 値を取り出す
    for (int i = 0; i < expectedQueueLength; i++) {
        int value = 0;
        EXPECT_EQ(queue.dequeue(&value), OperationResult::Success, result);
        EXPECT_EQ(value, i + 1, result);
    }

    // 範囲外の取り出しとなるか?
    int dummy = 0;
    EXPECT_EQ(queue.dequeue(&dummy), OperationResult::Empty, result);
    EndTestcase(result);
}

TEST(testOperateWithZeroSize) {
    BeginTestcase(result);
    // サイズ0のキュー
    Queue<int> queue(nullptr, 0);

    // 値の追加・取り出しに失敗する
    int value = 1;
    EXPECT_EQ(queue.enqueue(value), OperationResult::Overflow, result);
    EXPECT_EQ(queue.dequeue(&value), OperationResult::Empty, result);
    EndTestcase(result);
}

TEST(testOperateWithOneSize) {
    BeginTestcase(result);
    // サイズ1のキュー
    const int queueLength = 1;
    int queueData[queueLength] = {0};
    Queue<int> queue(queueData, 1);

    // 値の追加・取り出しが問題なく成功するか
    int value = 1;
    EXPECT_EQ(queue.enqueue(value), OperationResult::Success, result);
    EXPECT_EQ(queue.enqueue(value + 1), OperationResult::Overflow, result);

    value = 0;
    EXPECT_EQ(queue.dequeue(&value), OperationResult::Success, result);
    EXPECT_EQ(value, 1, result);
    EXPECT_EQ(queue.dequeue(&value), OperationResult::Empty, result);
    EndTestcase(result);
}

const TestFunction tests[] = {
    testInstantiate,
    testCapacityAndAmount,
    testOperateWithOrdinallySize,
    testOperateWithExceptionalSize,
    testOperateWithZeroSize,
    testOperateWithOneSize,
};

const size_t testCount = 6;

}  // namespace collection2tests

//
// キューのテスト
//

#include <gtest/gtest.h>

#include "collection2/queue.hpp"

using namespace collection2;

TEST(QueueTest, testInstantiate) {
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
}

TEST(QueueTest, testOperateWithOrdinallySize) {
    // int型を扱う長さ256のキューを定義
    const int queueLength = 256;
    int queueData[queueLength] = {0};
    Queue<int> queue(queueData, queueLength);

    // キューの長さだけ値を追加
    for (int i = 0; i < queueLength; i++) {
        EXPECT_EQ(queue.enqueue(i + 1), OperationResult::Success);
    }

    // 範囲外の追加
    EXPECT_EQ(queue.enqueue(0), OperationResult::Overflow);

    // 値を取り出す
    for (int i = 0; i < queueLength; i++) {
        int value = 0;
        EXPECT_EQ(queue.dequeue(&value), OperationResult::Success);
        EXPECT_EQ(value, i + 1);
    }

    // 範囲外の取り出し
    int dummy = 0;
    EXPECT_EQ(queue.dequeue(&dummy), OperationResult::Empty);
}

TEST(QueueTest, testOperateWithExceptionalSize) {
    // 2の冪数でない場合…
    const int queueLength = 100;
    int queueData[queueLength] = {0};
    Queue<int> queue(queueData, queueLength);

    // キューの長さに最も近い2の冪数回だけ値を追加
    const int expectedQueueLength = 64;
    for (int i = 0; i < expectedQueueLength; i++) {
        EXPECT_EQ(queue.enqueue(i + 1), OperationResult::Success);
    }

    // 範囲外となるか?
    EXPECT_EQ(queue.enqueue(0), OperationResult::Overflow);

    // 値を取り出す
    for (int i = 0; i < expectedQueueLength; i++) {
        int value = 0;
        EXPECT_EQ(queue.dequeue(&value), OperationResult::Success);
        EXPECT_EQ(value, i + 1);
    }

    // 範囲外の取り出しとなるか?
    int dummy = 0;
    EXPECT_EQ(queue.dequeue(&dummy), OperationResult::Empty);
}

TEST(QueueTest, testOperateWithZeroSize) {
    // サイズ0のキュー
    Queue<int> queue(nullptr, 0);

    // 値の追加・取り出しに失敗する
    int value = 1;
    EXPECT_EQ(queue.enqueue(value), OperationResult::Overflow);
    EXPECT_EQ(queue.dequeue(&value), OperationResult::Empty);
}

TEST(QueueTest, testOperateWithOneSize) {
    // サイズ1のキュー
    const int queueLength = 1;
    int queueData[queueLength] = {0};
    Queue<int> queue(queueData, 1);

    // 値の追加・取り出しが問題なく成功するか
    int value = 1;
    EXPECT_EQ(queue.enqueue(value), OperationResult::Success);
    EXPECT_EQ(queue.enqueue(value + 1), OperationResult::Overflow);

    value = 0;
    EXPECT_EQ(queue.dequeue(&value), OperationResult::Success);
    EXPECT_EQ(value, 1);
    EXPECT_EQ(queue.dequeue(&value), OperationResult::Empty);
}

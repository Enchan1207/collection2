//
// リストのテスト
//

#include "collection2/list.hpp"
#include "testcase.hpp"

using namespace collection2;

namespace collection2tests {

// head, tailから順にアクセス
TEST(testChainAccess) {
    BeginTestcase(result);
    const int listLength = 10;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    for (int i = 0; i < listLength; i++) {
        list.append(i + 1);
    }

    // 頭からアクセス
    auto* head = list.head();
    int expect = 1;
    EXPECT_TRUE(head != nullptr, result);
    while (head != nullptr) {
        EXPECT_EQ(head->element, expect, result);
        expect++;
        head = head->next;
    }
    EXPECT_EQ(expect, listLength + 1, result);

    // 後ろからアクセス
    auto* tail = list.tail();
    expect = listLength;
    EXPECT_TRUE(tail != nullptr, result);
    while (tail != nullptr) {
        EXPECT_EQ(tail->element, expect, result);
        expect--;
        tail = tail->previous;
    }
    EXPECT_EQ(expect, 0, result);
    EndTestcase(result);
}

// バッファ長
TEST(testCapacityAndAmount) {
    BeginTestcase(result);
    Node<int> data[10];
    List<int> list(data, 10);

    EXPECT_EQ(list.capacity(), 10, result);

    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);
    list.append(5);

    EXPECT_EQ(list.amount(), 5, result);
    EndTestcase(result);
}

// インデックス境界
TEST(testBoundaryIndex) {
    BeginTestcase(result);
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);
    for (int i = 0; i < listLength; i++) {
        EXPECT_EQ(list.append(i + 1), OperationResult::Success, result);
    }

    // リスト長の境界で遊ぶ
    EXPECT_TRUE(list.get(listLength + 1) == nullptr, result);
    EXPECT_TRUE(list.get(listLength) == nullptr, result);
    EXPECT_EQ(*list.get(listLength - 1), listLength, result);
    EXPECT_EQ(*list.get(0), 1, result);
    EndTestcase(result);
}

// 追加テスト1 (appendで構成 + insertで間に挿入 -> 連番)
TEST(testInsertion1) {
    BeginTestcase(result);
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    // リスト長の半分だけ値を末尾に挿入
    for (int i = 0; i < listLength; i += 2) {
        EXPECT_EQ(list.append(i + 1), OperationResult::Success, result);
    }

    // 確認
    int expected[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    for (int i = 0; i < listLength / 2; i++) {
        const auto* value = list.get(i);
        EXPECT_FALSE(value == nullptr, result);
        EXPECT_EQ(expected[i], *value, result);
    }

    // 一つ飛ばしで間に挿入
    for (int i = 0; i < listLength / 2; i++) {
        EXPECT_EQ(list.insert(expected[i], (i + 1) * 2), OperationResult::Success, result);
    }

    // 連番になっているはず
    for (int i = 0; i < listLength; i++) {
        const auto* value = list.get(i);
        EXPECT_FALSE(value == nullptr, result);
        EXPECT_EQ(i + 1, *value, result);
    }
    EndTestcase(result);
}

// 追加テスト2 (insertで先頭に挿入 -> 連番)
TEST(testInsertion2) {
    BeginTestcase(result);
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    // 逆順に先頭へ挿入
    for (int i = 0; i < listLength; i++) {
        EXPECT_EQ(list.insert(0, listLength - i), OperationResult::Success, result);
    }

    // 連番になっているはず
    for (int i = 0; i < listLength; i++) {
        const auto* value = list.get(i);
        EXPECT_FALSE(value == nullptr, result);
        EXPECT_EQ(i + 1, *value, result);
    }
    EndTestcase(result);
}

// 削除テスト1 (末尾から削除 -> 連番)
TEST(testDeletion1) {
    BeginTestcase(result);
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    // 逆順に末尾へ挿入
    for (int i = 0; i < listLength; i++) {
        EXPECT_EQ(list.append(listLength - i), OperationResult::Success, result);
    }

    // 削除+取り出し 連番になっているはず
    for (int i = 0; i < listLength; i++) {
        auto value = -1;
        EXPECT_EQ(list.pop(&value), OperationResult::Success, result);
        EXPECT_EQ(i + 1, value, result);
    }
    EndTestcase(result);
}

// 削除テスト2 (先頭から削除 -> 連番)
TEST(testDeletion2) {
    BeginTestcase(result);
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    // 末尾へ挿入
    for (int i = 0; i < listLength; i++) {
        EXPECT_EQ(list.append(i + 1), OperationResult::Success, result);
    }

    // 先頭から削除+取り出し 連番になっているはず
    for (int i = 0; i < listLength; i++) {
        auto value = -1;
        EXPECT_EQ(list.remove(0, &value), OperationResult::Success, result);
        EXPECT_EQ(i + 1, value, result);
    }
    EndTestcase(result);
}

// さまざまなパターンでの追加・削除
TEST(testAdditionAndDeletion) {
    BeginTestcase(result);
    const int listLength = 10;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    int value = 123;
    EXPECT_EQ(list.append(value), OperationResult::Success, result);
    EXPECT_EQ(*list.get(0), 123, result);
    value = 0;
    EXPECT_EQ(list.pop(&value), OperationResult::Success, result);
    EXPECT_EQ(value, 123, result);

    EXPECT_EQ(list.append(456), OperationResult::Success, result);     // 4, result56
    EXPECT_EQ(list.insert(0, 789), OperationResult::Success, result);  // 789, 4, result56
    EXPECT_EQ(list.insert(1, 123), OperationResult::Success, result);  // 789, 123, 4, result56
    EXPECT_EQ(list.append(1234), OperationResult::Success, result);    // 789, 123, 456, 12, result34

    EXPECT_EQ(*list.get(0), 789, result);
    EXPECT_EQ(list.remove(3, &value), OperationResult::Success, result);
    EXPECT_EQ(value, 1234, result);
    EXPECT_EQ(list.remove(2, &value), OperationResult::Success, result);
    EXPECT_EQ(value, 456, result);
    EXPECT_EQ(list.pop(&value), OperationResult::Success, result);
    EXPECT_EQ(value, 123, result);
    EndTestcase(result);
}

// 長さゼロのリスト
TEST(testOperateWithZeroSize) {
    BeginTestcase(result);
    List<int> list(nullptr, 0);

    // 値の追加・取り出しに失敗する
    int value = 1;
    EXPECT_EQ(list.append(value), OperationResult::Overflow, result);
    EXPECT_EQ(list.pop(&value), OperationResult::Empty, result);
    EndTestcase(result);
}

// 長さ1のリスト
TEST(testOperateWithOneSize) {
    BeginTestcase(result);
    const int listLength = 1;
    Node<int> listData[listLength];
    List<int> list(listData, 1);

    // 値の追加・取り出しが問題なく成功するか
    EXPECT_EQ(list.append(1), OperationResult::Success, result);
    EXPECT_EQ(list.append(2), OperationResult::Overflow, result);
    EXPECT_EQ(list.insert(0, 3), OperationResult::Overflow, result);
    EXPECT_EQ(list.insert(1, 4), OperationResult::Overflow, result);

    int value = 0;
    EXPECT_EQ(list.pop(&value), OperationResult::Success, result);
    EXPECT_EQ(value, 1, result);
    EXPECT_EQ(list.pop(&value), OperationResult::Empty, result);
    EXPECT_EQ(list.remove(0, &value), OperationResult::Empty, result);
    EXPECT_EQ(list.remove(1, &value), OperationResult::Empty, result);
    EndTestcase(result);
}

const TestFunction tests[] = {
    testChainAccess,
    testCapacityAndAmount,
    testBoundaryIndex,
    testInsertion1,
    testInsertion2,
    testDeletion1,
    testDeletion2,
    testAdditionAndDeletion,
    testOperateWithZeroSize,
    testOperateWithOneSize,
};

const size_t testCount = 10;

}  // namespace collection2tests

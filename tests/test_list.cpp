//
// リストのテスト
//

#include <gtest/gtest.h>

#include "collection2/list.hpp"

using namespace collection2;

// インスタンス化
TEST(ListTest, testInstantiate) {
    // プリミティブ型
    Node<char> charListData[8];
    List<char> charList(charListData, 8);

    // 構造体
    struct Structure {
        int integer;
        char charactor;
    };
    Node<Structure> structListData[8];
    List<Structure> structBuffer(structListData, 8);
}

// head, tailから順にアクセス
TEST(ListTest, testChainAccess) {
    const int listLength = 10;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    for (int i = 0; i < listLength; i++) {
        list.append(i + 1);
    }

    // 頭からアクセス
    auto* head = list.head();
    int expect = 1;
    EXPECT_TRUE(head != nullptr);
    while (head != nullptr) {
        EXPECT_EQ(head->element, expect);
        expect++;
        head = head->next;
    }
    EXPECT_EQ(expect, listLength + 1);

    // 後ろからアクセス
    auto* tail = list.tail();
    expect = listLength;
    EXPECT_TRUE(tail != nullptr);
    while (tail != nullptr) {
        EXPECT_EQ(tail->element, expect);
        expect--;
        tail = tail->previous;
    }
    EXPECT_EQ(expect, 0);
}

// インデックス境界
TEST(ListTest, testBoundaryIndex) {
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);
    for (int i = 0; i < listLength; i++) {
        EXPECT_EQ(list.append(i + 1), OperationResult::Success);
    }

    // リスト長の境界で遊ぶ
    EXPECT_TRUE(list.get(listLength + 1) == nullptr);
    EXPECT_TRUE(list.get(listLength) == nullptr);
    EXPECT_EQ(*list.get(listLength - 1), listLength);
    EXPECT_EQ(*list.get(0), 1);
}

// 追加テスト1 (appendで構成 + insertで間に挿入 -> 連番)
TEST(ListTest, testInsertion1) {
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    // リスト長の半分だけ値を末尾に挿入
    for (int i = 0; i < listLength; i += 2) {
        EXPECT_EQ(list.append(i + 1), OperationResult::Success);
    }

    // 確認
    int expected[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    for (list_size_t i = 0; i < listLength / 2; i++) {
        const auto* value = list.get(i);
        EXPECT_FALSE(value == nullptr);
        EXPECT_EQ(expected[i], *value);
    }

    // 一つ飛ばしで間に挿入
    for (list_size_t i = 0; i < listLength / 2; i++) {
        EXPECT_EQ(list.insert(expected[i], (i + 1) * 2), OperationResult::Success);
    }

    // 連番になっているはず
    for (list_size_t i = 0; i < listLength; i++) {
        const auto* value = list.get(i);
        EXPECT_FALSE(value == nullptr);
        EXPECT_EQ(i + 1, *value);
    }
}

// 追加テスト2 (insertで先頭に挿入 -> 連番)
TEST(ListTest, testInsertion2) {
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    // 逆順に先頭へ挿入
    for (int i = 0; i < listLength; i++) {
        EXPECT_EQ(list.insert(0, listLength - i), OperationResult::Success);
    }

    // 連番になっているはず
    for (list_size_t i = 0; i < listLength; i++) {
        const auto* value = list.get(i);
        EXPECT_FALSE(value == nullptr);
        EXPECT_EQ(i + 1, *value);
    }
}

// 削除テスト1 (末尾から削除 -> 連番)
TEST(ListTest, testDeletion1) {
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    // 逆順に末尾へ挿入
    for (int i = 0; i < listLength; i++) {
        EXPECT_EQ(list.append(listLength - i), OperationResult::Success);
    }

    // 削除+取り出し 連番になっているはず
    for (list_size_t i = 0; i < listLength; i++) {
        auto value = -1;
        EXPECT_EQ(list.pop(&value), OperationResult::Success);
        EXPECT_EQ(i + 1, value);
    }
}

// 削除テスト2 (先頭から削除 -> 連番)
TEST(ListTest, testDeletion2) {
    const int listLength = 20;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    // 末尾へ挿入
    for (int i = 0; i < listLength; i++) {
        EXPECT_EQ(list.append(i + 1), OperationResult::Success);
    }

    // 先頭から削除+取り出し 連番になっているはず
    for (list_size_t i = 0; i < listLength; i++) {
        auto value = -1;
        EXPECT_EQ(list.remove(0, &value), OperationResult::Success);
        EXPECT_EQ(i + 1, value);
    }
}

// さまざまなパターンでの追加・削除
TEST(ListTest, testAdditionAndDeletion) {
    const int listLength = 10;
    Node<int> listData[listLength];
    List<int> list(listData, listLength);

    int value = 123;
    EXPECT_EQ(list.append(value), OperationResult::Success);
    EXPECT_EQ(*list.get(0), 123);
    value = 0;
    EXPECT_EQ(list.pop(&value), OperationResult::Success);
    EXPECT_EQ(value, 123);

    EXPECT_EQ(list.append(456), OperationResult::Success);     // 456
    EXPECT_EQ(list.insert(0, 789), OperationResult::Success);  // 789, 456
    EXPECT_EQ(list.insert(1, 123), OperationResult::Success);  // 789, 123, 456
    EXPECT_EQ(list.append(1234), OperationResult::Success);    // 789, 123, 456, 1234

    EXPECT_EQ(*list.get(0), 789);
    EXPECT_EQ(list.remove(3, &value), OperationResult::Success);
    EXPECT_EQ(value, 1234);
    EXPECT_EQ(list.remove(2, &value), OperationResult::Success);
    EXPECT_EQ(value, 456);
    EXPECT_EQ(list.pop(&value), OperationResult::Success);
    EXPECT_EQ(value, 123);
}

// 長さゼロのリスト
TEST(ListTest, testOperateWithZeroSize) {
    List<int> list(nullptr, 0);

    // 値の追加・取り出しに失敗する
    int value = 1;
    EXPECT_EQ(list.append(value), OperationResult::Overflow);
    EXPECT_EQ(list.pop(&value), OperationResult::Empty);
}

// 長さ1のリスト
TEST(ListTest, testOperateWithOneSize) {
    const int listLength = 1;
    Node<int> listData[listLength];
    List<int> list(listData, 1);

    // 値の追加・取り出しが問題なく成功するか
    EXPECT_EQ(list.append(1), OperationResult::Success);
    EXPECT_EQ(list.append(2), OperationResult::Overflow);
    EXPECT_EQ(list.insert(0, 3), OperationResult::Overflow);
    EXPECT_EQ(list.insert(1, 4), OperationResult::Overflow);

    int value = 0;
    EXPECT_EQ(list.pop(&value), OperationResult::Success);
    EXPECT_EQ(value, 1);
    EXPECT_EQ(list.pop(&value), OperationResult::Empty);
    EXPECT_EQ(list.remove(0, &value), OperationResult::Empty);
    EXPECT_EQ(list.remove(1, &value), OperationResult::Empty);
}

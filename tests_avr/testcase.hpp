//
//
//
#ifndef COLLECTION2_TESTCASE_H
#define COLLECTION2_TESTCASE_H

#include <stddef.h>

#include <uart/uart.hpp>

/**
 * @brief テストケース
 */
#define TEST(testName) bool testName(void)

#define BeginTestcase(result) bool result = true
#define EndTestcase(result) return result

#define EXPECT(expressionstr, expression, result) result = collection2tests::expect(expressionstr, expression)
#define EXPECT_EQ(lhs, rhs, result) EXPECT(#lhs " == " #rhs, lhs == rhs, result)
#define EXPECT_NE(lhs, rhs, result) EXPECT(#lhs " != " #rhs, lhs != rhs, result)
#define EXPECT_TRUE(expression, result) EXPECT(#expression " == true", expression, result)
#define EXPECT_FALSE(expression, result) EXPECT(#expression " == false", !(expression), result)

namespace collection2tests {

/**
 * @brief テストケースの関数ポインタ
 */
using TestFunction = bool (*)(void);

/**
 * @brief テスト対象関数の配列
 */
extern const TestFunction tests[];

/**
 * @brief testsの要素数(=テストケースの数)
 */
extern const size_t testCount;

/**
 * @brief 式がtrueになることを期待する
 *
 * @param expressionStr 評価する式につけるラベル
 * @param expression 評価する式
 * @return bool 評価結果
 */
bool expect(const char* const expressionStr, bool expression);

}  // namespace collection2tests

#endif /* COLLECTION2_TESTCASE_H */

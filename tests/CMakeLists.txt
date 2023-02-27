#
# collection2 tests
#
cmake_minimum_required(VERSION 3.0)

# GoogleTestのフェッチと有効化
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

## 外部依存関係を追加...

# テストターゲットの構成
add_executable(collection2_test)
target_sources(collection2_test PRIVATE
    test_queue.cpp
    test_buffer.cpp
    test_list.cpp
    test_stack.cpp
)
target_link_libraries(collection2_test PRIVATE
    GTest::gtest_main
)
target_include_directories(collection2_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

# テストの実行
gtest_discover_tests(collection2_test)

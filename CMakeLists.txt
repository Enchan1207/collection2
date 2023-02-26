#
# collection2
#
cmake_minimum_required(VERSION 3.0)
project(collection2
    VERSION 0.1.0
    DESCRIPTION "collection of data structures"
    LANGUAGES C CXX
)

add_subdirectory(src)

# 依存関係としてビルドされている場合は単体テストディレクトリをaddしない
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(COLLECTION2_BUILD_TESTS "whether to build tests" ON)

    if(COLLECTION2_BUILD_TESTS)
        message(NOTICE "testing enabled")
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()

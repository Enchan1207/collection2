#
# collection2
#
cmake_minimum_required(VERSION 3.0)
project(collection2
    VERSION 0.1.0
    DESCRIPTION "collection of data structures"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ライブラリの構成
add_library(collection2 INTERFACE)
target_include_directories(collection2 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 依存関係としてビルドされている場合は単体テストディレクトリをaddしない
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(COLLECTION2_BUILD_TESTS "whether to build tests" ON)

    if(COLLECTION2_BUILD_TESTS)
        message(NOTICE "testing enabled")
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()

# インストール構成

# configファイル (collection2-config.cmake) を生成
install(TARGETS collection2
    EXPORT collection2-config
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ヘッダファイルをインストール
install(DIRECTORY ${collection2_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

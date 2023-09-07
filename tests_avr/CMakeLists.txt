#
# collection2 tests for AVR microcontroller
#
cmake_minimum_required(VERSION 3.0)

# AVR用UARTライブラリの導入
include(FetchContent)
FetchContent_Declare(
    avr-serial
    GIT_REPOSITORY https://github.com/Enchan1207/avr-serial
    GIT_TAG v0.1.1
)
FetchContent_MakeAvailable(avr-serial)

# AVR用テストターゲットジェネレータ
macro(add_test_target source_file_name)
    get_filename_component(target_name "${source_file_name}" NAME_WE)
    add_executable(${target_name})
    target_sources(${target_name} PRIVATE
        isrs.cpp
        ${source_file_name}
    )
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_compile_options(${target_name} PRIVATE -Wall)
    target_link_libraries(${target_name} PRIVATE usart)
endmacro()

# テストターゲットの構成
add_test_target(test_buffer.cpp)
add_test_target(test_list.cpp)
add_test_target(test_queue.cpp)
add_test_target(test_stack.cpp)

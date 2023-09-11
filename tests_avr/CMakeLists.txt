#
# collection2 tests for AVR microcontroller
#
cmake_minimum_required(VERSION 3.0)

# AVR用UARTライブラリの導入
include(FetchContent)
FetchContent_Declare(
    avr-serial
    GIT_REPOSITORY https://github.com/Enchan1207/avr-serial
    GIT_TAG v0.2.0
)
FetchContent_MakeAvailable(avr-serial)

# QEMUの -machine に渡す値と avr-gccの -mmcu に渡す値のマップ
set(qemu_machine_map_atmega168 arduino-duemilanove)
set(qemu_machine_map_atmega168p arduino-duemilanove)
set(qemu_machine_map_atmega328 arduino-uno)
set(qemu_machine_map_atmega328p arduino-uno)
set(qemu_machine_map_atmega1280 arduino-mega)
set(qemu_machine_map_atmega2560 arduino-mega-2560-v3)

# AVR用テストターゲットジェネレータ
macro(add_test_target source_file_name)
    get_filename_component(target_name "${source_file_name}" NAME_WE)
    add_executable(${target_name})
    target_sources(${target_name} PRIVATE
        isrs.cpp
        testcase.cpp
        ${source_file_name}
    )
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_compile_options(${target_name} PRIVATE -Wall)
    target_link_libraries(${target_name} PRIVATE uart)
    target_configure_for_avr(${target_name})
    add_test(NAME assert_${target_name} COMMAND ${PROJECT_SOURCE_DIR}/tests_avr/testcase_capture.py -M ${qemu_machine_map_${AVR_MCU}} -bios ${target_name})
endmacro()

# テストターゲットの構成
add_test_target(test_buffer.cpp)
add_test_target(test_list.cpp)
add_test_target(test_queue.cpp)
add_test_target(test_stack.cpp)

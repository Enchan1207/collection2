#
# collection2
#

#
# project configuration
#
cmake_minimum_required(VERSION 3.0)
project(collection2
    VERSION 0.2.0
    DESCRIPTION "collection of data structures"
    LANGUAGES C CXX
)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#
# exported library configuration
#
add_library(collection2 INTERFACE)
target_include_directories(collection2 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
install(
    DIRECTORY ${collection2_SOURCE_DIR}/include/collection2
    DESTINATION include
)


#
# unit testing configuration
#
option(COLLECTION2_BUILD_TESTS "Enable testing of collection2" OFF)
if(COLLECTION2_BUILD_TESTS)
    set(INSTALL_GTEST OFF)
    enable_testing()
    message(NOTICE "collection2: unit testing is enabled")
    
    add_subdirectory(tests)
endif()

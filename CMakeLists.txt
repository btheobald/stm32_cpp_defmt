cmake_minimum_required(VERSION 3.15)

set(CHIP_CORE cortex-m0plus)

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/utl/stm32f072.ld)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/utl/arm-gcc.cmake)

project(cpp_test)

enable_language(C CXX)

set(TARGET_NAME ${CMAKE_PROJECT_NAME})

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILT_TYPE Release)
endif()

add_subdirectory(ext)
add_subdirectory(src)

include(${CMAKE_CURRENT_LIST_DIR}/utl/logview.cmake)

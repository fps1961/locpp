cmake_minimum_required(VERSION 3.30)

project(locpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)

add_executable(locpp ${SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(locpp PRIVATE -g)
endif ()

enable_testing()


add_test(NAME TestTokenization1
        COMMAND bash -c "${CMAKE_BINARY_DIR}/locpp ${CMAKE_CURRENT_SOURCE_DIR}/tests/ch4/test1.lox > actual_output.txt && diff -u actual_output.txt ${CMAKE_CURRENT_SOURCE_DIR}/tests/ch4/test1.expected || (echo 'Test failed, see diff above' && exit 1)"
)

add_test(NAME TestTokenization2
        COMMAND bash -c "${CMAKE_BINARY_DIR}/locpp ${CMAKE_CURRENT_SOURCE_DIR}/tests/ch4/test2.lox > actual_output.txt && diff -u actual_output.txt ${CMAKE_CURRENT_SOURCE_DIR}/tests/ch4/test2.expected || (echo 'Test failed, see diff above' && exit 1)"
)
cmake_minimum_required(VERSION 3.30)

project(locpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_library(locpp_lib ${SOURCES})

add_executable(locpp src/main.cpp)
target_link_libraries(locpp PRIVATE locpp_lib)

add_executable(print_ast scripts/AstPrinter.cpp)
target_link_libraries(print_ast PRIVATE locpp_lib)

add_executable(generate_ast scripts/GenerateAst.cpp)
target_link_libraries(generate_ast PRIVATE locpp_lib)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(locpp PRIVATE -g)
endif ()

enable_testing()

add_test(NAME TestTokenization1
        COMMAND bash -c "${CMAKE_BINARY_DIR}/locpp ${CMAKE_CURRENT_SOURCE_DIR}/tests/TestTokenization1.lox > actual_output.txt && diff -u actual_output.txt ${CMAKE_CURRENT_SOURCE_DIR}/tests/TestTokenization1.expected || (echo 'Test failed, see diff above' && exit 1)"
)

add_test(NAME TestTokenization2
        COMMAND bash -c "${CMAKE_BINARY_DIR}/locpp ${CMAKE_CURRENT_SOURCE_DIR}/tests/TestTokenization2.lox> actual_output.txt && diff -u actual_output.txt ${CMAKE_CURRENT_SOURCE_DIR}/tests/TestTokenization2.expected || (echo 'Test failed, see diff above' && exit 1)"
)

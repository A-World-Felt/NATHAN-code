cmake_minimum_required(VERSION 3.16)

project(MyGameEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

# Engine library
add_library(gameplay_foundations
    gameplay_foundations/engine.cpp
    gameplay_foundations/node.cpp
)

target_include_directories(gameplay_foundations PUBLIC
    ${CMAKE_SOURCE_DIR}/gameplay_foundations
)

# Test executable
add_executable(test_game
    test/main.cpp
    test/main-scene.cpp
)

target_link_libraries(test_game PRIVATE gameplay_foundations)
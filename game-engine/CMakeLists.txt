cmake_minimum_required(VERSION 3.16)

project(MyGameEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

# SDL Build Flags
set(SDL_UNIX_CONSOLE_BUILD ON CACHE BOOL "" FORCE)
set(SDL_GAMEPAD ON CACHE BOOL "" FORCE)
set(SDL_JOYSTICK ON CACHE BOOL "" FORCE)
set(SDL_HAPTIC ON CACHE BOOL "" FORCE)

set(SDL_AUDIO OFF CACHE BOOL "" FORCE)
set(SDL_CAMERA OFF CACHE BOOL "" FORCE)
set(SDL_SENSOR OFF CACHE BOOL "" FORCE)
set(SDL_VIDEO OFF CACHE BOOL "" FORCE)
set(SDL_GPU OFF CACHE BOOL "" FORCE)
set(SDL_RENDER OFF CACHE BOOL "" FORCE)

# Engine library
add_library(gameplay_foundations
    # Events
    gameplay_foundations/events/connection.hpp
    gameplay_foundations/events/event_emitter.hpp
    gameplay_foundations/events/event_bus.hpp
    
    # Node
    gameplay_foundations/node/node.hpp
    gameplay_foundations/node/node.inl.hpp
    gameplay_foundations/node/event_node.hpp
    gameplay_foundations/node/scene_manager.hpp
    gameplay_foundations/node/scene_manager.cpp
    
    # Engine
    gameplay_foundations/engine/engine.hpp
    gameplay_foundations/engine/engine.cpp
    gameplay_foundations/engine/node_pool.hpp
    gameplay_foundations/engine/node_pool.cpp
    
    # Resources
    gameplay_foundations/resources/resource.hpp
    gameplay_foundations/resources/resource_manager.hpp
    gameplay_foundations/resources/resource_manager.cpp
    gameplay_foundations/resources/text_resource.hpp
    gameplay_foundations/resources/text_resource.cpp
    gameplay_foundations/resources/config.hpp
    gameplay_foundations/resources/config.cpp
    gameplay_foundations/resources/config_manager.hpp
    gameplay_foundations/resources/config_manager.cpp

    # Inputs
    gameplay_foundations/inputs/iinput_device.hpp
    gameplay_foundations/inputs/sdl/sdl_input_device.cpp
    gameplay_foundations/inputs/sdl/sdl_input_device.hpp
    gameplay_foundations/inputs/input_types.hpp
)

target_include_directories(gameplay_foundations PUBLIC
    ${CMAKE_SOURCE_DIR}/gameplay_foundations
    ${CMAKE_SOURCE_DIR}/gameplay_foundations/node
    ${CMAKE_SOURCE_DIR}/gameplay_foundations/events
    ${CMAKE_SOURCE_DIR}/gameplay_foundations/engine
    ${CMAKE_SOURCE_DIR}/gameplay_foundations/resources
    ${CMAKE_SOURCE_DIR}/vendor/SDL/include
    ${CMAKE_SOURCE_DIR}/vendor/toml++
)

# Coverage option
option(ENABLE_COVERAGE "Enable code coverage with gcov" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(gameplay_foundations PRIVATE --coverage)
    target_link_options(gameplay_foundations PRIVATE --coverage)
endif()

# Tests option
option(BUILD_TESTING "Build tests" ON)

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

target_link_libraries(gameplay_foundations PRIVATE SDL3::SDL3)

# Demo executable
add_executable(demo_game
    demo/main.cpp
    demo/scenes/scene_manager.hpp
    demo/scenes/scene_manager.cpp
    demo/scenes/main_menu.hpp
    demo/scenes/main_menu.cpp
    demo/scenes/mini_game.hpp
    demo/scenes/mini_game.cpp
)

if(WIN32)
    add_custom_command(TARGET demo_game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:demo_game>
        $<TARGET_FILE_DIR:demo_game>
        COMMAND_EXPAND_LISTS
    )
endif()

target_link_libraries(demo_game PRIVATE gameplay_foundations)

if(ENABLE_COVERAGE)
    target_compile_options(demo_game PRIVATE --coverage)
    target_link_options(demo_game PRIVATE --coverage)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

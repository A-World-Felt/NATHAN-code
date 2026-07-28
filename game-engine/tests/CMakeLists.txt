# Tests for NATHAN game engine
# This file is included by the root CMakeLists.txt

include(FetchContent)

# Fetch Google Test - use latest stable version
# As of 2026, the latest stable is v1.17.x
# Check https://github.com/google/googletest/releases for the latest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0  # Latest stable as of 2026
)
FetchContent_MakeAvailable(googletest)

# Collect all test source files
file(GLOB_RECURSE TEST_SOURCES "unit/**/*.cpp")

# Create test executable
add_executable(game_engine_tests ${TEST_SOURCES})

# Link libraries
target_link_libraries(game_engine_tests
    PRIVATE
    gameplay_foundations
    gtest
    gtest_main
    gmock
)

# Enable coverage for our test code
if(ENABLE_COVERAGE)
    target_compile_options(game_engine_tests PRIVATE --coverage)
    target_link_options(game_engine_tests PRIVATE --coverage)
endif()

# Use C++20
target_compile_features(game_engine_tests PRIVATE cxx_std_20)

# Include directories - inherit from linked targets, add Google Test paths
target_include_directories(game_engine_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/tests
    ${gtest_SOURCE_DIR}
    ${gmock_SOURCE_DIR}
)

if (WIN32)
    add_custom_command(TARGET game_engine_tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:game_engine_tests>
            $<TARGET_FILE_DIR:game_engine_tests>
            COMMAND_EXPAND_LISTS
    )
endif()

# Add test to CTest - auto-discover individual tests
include(GoogleTest)
gtest_discover_tests(game_engine_tests)

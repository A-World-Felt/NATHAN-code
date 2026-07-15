# NATHAN Game Engine - Unit Tests

Unit tests using **Google Test v1.17.0** and **Google Mock**.

## Quick Start

### Build and Run All Tests
```bash
cmake -S . -B build
cmake --build build
./build/tests/game_engine_tests
```

### Run Specific Test Suite
```bash
# Run only Node tests
./build/tests/game_engine_tests --gtest_filter=NodeTest.*

# Run only engine tests
./build/tests/game_engine_tests --gtest_filter=EngineTest.*

# Run tests matching a pattern
./build/tests/game_engine_tests --gtest_filter=*Event*
```

## Test Structure

```
tests/unit/gameplay_foundations/
├── engine/
│   ├── engine_test.cpp
│   └── node_pool_test.cpp
├── node/
│   ├── node_test.cpp
│   ├── event_node_test.cpp
│   ├── scene_manager_test.cpp
│   └── node_with_mock_test.cpp
└── events/
    ├── connection_test.cpp
    ├── event_bus_test.cpp
    └── event_emitter_test.cpp
```

**55 tests across 9 files**

## Writing Tests

### Create a New Test File
1. Create a `.cpp` file in the appropriate `tests/unit/gameplay_foundations/` subdirectory
2. Include Google Test and your module header

```cpp
#include <gtest/gtest.h>
#include "gameplay_foundations/your_module/your_header.hpp"

TEST(ModuleTest, TestName) {
    // Arrange
    YourClass obj;
    
    // Act
    auto result = obj.method();
    
    // Assert
    EXPECT_EQ(result, expected_value);
    EXPECT_TRUE(obj.is_valid());
}
```

### Example: Testing Node
```cpp
#include <gtest/gtest.h>
#include "gameplay_foundations/node/node.hpp"

TEST(NodeTest, NameAccessors) {
    nathan::Node node;
    node.set_name("test");
    EXPECT_EQ(node.get_name(), "test");
}
```

## Code Coverage

### HTML Report (lcov + genhtml)
```bash
# Install
# Ubuntu/Debian:
#   sudo apt install lcov genhtml
# Windows (Chocolatey):
#   choco install lcov

# Generate HTML report (outputs to tests/coverage_html/)
cmake -S . -B build -DENABLE_COVERAGE=ON
cmake --build build
./build/tests/game_engine_tests
lcov --capture --directory build --output-file tests/coverage.info \
  --exclude "/usr/include/*" \
  --exclude "*/_deps/*" \
  --exclude "*tests*"
genhtml tests/coverage.info --output-directory tests/coverage_html
# Open tests/coverage_html/index.html in your browser
# Note: tests/coverage_html/ is in .gitignore, so coverage files won't be committed
```

## VSCode Integration

### Tasks
- **Run Tests**: Run all tests (Ctrl+Shift+P → Run Task → Run Tests)
- **Generate Coverage**: Generate HTML coverage report (requires lcov/genhtml)
- **Build, Test & Coverage**: Combined workflow

### Launch Configurations
- **Debug Tests**: Debug tests with breakpoints (requires C++ debugger)

## Resources

- [Google Test](https://google.github.io/googletest/)
- [Google Mock](https://google.github.io/googletest/gmock_for_dummies.html)

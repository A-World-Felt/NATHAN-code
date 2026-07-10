# Style Guide

This document outlines the C++ coding standards for the NATHAN game engine, based on the Google C++ Style Guide.

## Header Files

- Use `#ifndef` guards, not `#pragma once`
- Format: `#ifndef FILENAME_H_` / `#define FILENAME_H_` / `#endif  // FILENAME_H_`

## Namespaces

- Use a single `nathan` namespace for all project code
- No nested namespaces

## Includes

- **Library files**: Use short paths relative to CMake include directories
  - Example: `#include "node/node.hpp"` not `#include "gameplay_foundations/node/node.hpp"`
  - Do not use `.` or `..` in include paths
- **Demo/test files**: Use short paths for library includes, relative paths for demo includes
  - Library: `#include "node/event_node.hpp"`
  - Demo: `#include "scene_manager.hpp"` (same directory)

## Include Order

1. Related header (if exists)
2. Blank line
3. C++ standard library headers
4. Blank line
5. Project headers

## Data Members

- Make all data members `private`
- Provide public accessor methods (getters/setters) as needed

## Pointers

- Use raw pointers for non-owning references
- Use `std::unique_ptr` for exclusive ownership
- Use `std::shared_ptr` for shared ownership (rarely needed)

## Logging

- Use `std::cout` for simple logging and debug output
- A custom logging utility could be made later

## Naming

- Variables and functions: `snake_case`
- Types (classes, structs, enums): `PascalCase`
- Constants: `kConstantName`
- Member variables: `name_` (trailing underscore)

## Comments

- Use `//` for comments
- Keep comments brief and only when necessary
- Comments explain *why*, not *what*

## Example File Structure

```cpp
#ifndef NATHAN_NODE_NODE_H_
#define NATHAN_NODE_NODE_H_

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace nathan {

class Node {
public:
    std::string_view get_name() const { return name_; }
    void set_name(std::string name) { name_ = std::move(name); }

private:
    std::string name_;
};

}  // namespace nathan

#endif  // NATHAN_NODE_NODE_H_
```

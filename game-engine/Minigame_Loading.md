# NATHAN Game Engine: Minigame Loading Comparison

*Comprehensive analysis of C++ Dynamic Library vs Python vs Hybrid approaches for hot-pluggable minigames*

---

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [Requirements Analysis](#requirements-analysis)
3. [Approach Comparison Matrix](#approach-comparison-matrix)
4. [Detailed Architecture](#detailed-architecture)
   - [C++ Dynamic Library Approach](#1-c-dynamic-library-approach)
   - [Python Approach](#2-python-approach)
   - [Hybrid Approach](#3-hybrid-approach)
5. [Class Diagrams](#class-diagrams)
6. [Sequence Diagrams](#sequence-diagrams)
7. [Component Specifications](#component-specifications)
8. [File Structure Comparison](#file-structure-comparison)
9. [Build System Comparison](#build-system-comparison)
10. [Performance Analysis](#performance-analysis)
11. [Development Workflow Comparison](#development-workflow-comparison)
12. [Error Handling Strategies](#error-handling-strategies)
13. [Version Compatibility](#version-compatibility)
14. [Security Considerations](#security-considerations)
15. [Integration with Resource & Config Systems](#integration-with-resource--config-systems)
16. [Implementation Roadmap](#implementation-roadmap)
17. [Recommendations](#recommendations)

---

## Executive Summary

This document provides an exhaustive comparison of three approaches for implementing hot-pluggable minigames in the NATHAN Game Engine. The analysis covers technical architecture, implementation details, performance characteristics, and practical considerations for each approach.

### Quick Comparison

| Aspect | C++ Dynamic Library | Python | Hybrid |
|--------|---------------------|--------|--------|
| **Ease of Use** | ⭐⭐ (2/5) | ⭐⭐⭐⭐⭐ (5/5) | ⭐⭐⭐⭐ (4/5) |
| **Performance** | ⭐⭐⭐⭐⭐ (5/5) | ⭐⭐ (2/5) | ⭐⭐⭐⭐ (4/5) |
| **Development Speed** | ⭐⭐ (2/5) | ⭐⭐⭐⭐⭐ (5/5) | ⭐⭐⭐⭐ (4/5) |
| **Cross-Platform** | ⭐⭐⭐ (3/5) | ⭐⭐⭐⭐⭐ (5/5) | ⭐⭐⭐⭐⭐ (5/5) |
| **No Compilation Required** | ❌ No | ✅ Yes | ✅ Partial |
| **Hot Reload Capability** | ❌ No | ✅ Yes | ✅ Partial |
| **Memory Overhead** | ⭐⭐⭐⭐ (Low) | ⭐⭐ (High) | ⭐⭐⭐ (Medium) |
| **Startup Time** | ⭐⭐⭐⭐ (Fast) | ⭐⭐ (Slow) | ⭐⭐⭐ (Medium) |
| **Distribution Simplicity** | ⭐⭐⭐ (Binary files) | ⭐⭐⭐⭐⭐ (Source files) | ⭐⭐⭐⭐ (Both) |
| **Accessibility** | ⭐⭐ (Hard) | ⭐⭐⭐⭐⭐ (Easy) | ⭐⭐⭐⭐ (Easy) |
| **Implementation Complexity** | ⭐⭐⭐⭐ (4/5) | ⭐⭐⭐ (3/5) | ⭐⭐⭐⭐ (4/5) |

**Final Recommendation for NATHAN**: **Hybrid Approach with Python as Primary**

Start with Python support for ease of use and accessibility, add C++ dynamic loading later for users who need maximum performance. This provides the best balance for your target audience (developers including those with visual impairments) while maintaining future extensibility.

---

## Requirements Analysis

### Functional Requirements

| ID | Requirement | Priority | C++ | Python | Hybrid |
|----|-------------|----------|-----|--------|--------|
| R1 | Hot-pluggable minigames (no engine recompile) | MUST | ✅ | ✅ | ✅ |
| R2 | Cross-platform (Linux + Windows + macOS) | MUST | ⚠️ | ✅ | ✅ |
| R3 | Per-minigame asset isolation | MUST | ✅ | ✅ | ✅ |
| R4 | Resource sharing across minigames | MUST | ✅ | ✅ | ✅ |
| R5 | Relative path resolution | MUST | ✅ | ✅ | ✅ |
| R6 | Consistent API across all minigames | MUST | ✅ | ✅ | ✅ |
| R7 | Auto-discovery of available minigames | SHOULD | ✅ | ✅ | ✅ |
| R8 | Metadata support (name, version, description) | SHOULD | ✅ | ✅ | ✅ |
| R9 | Hot reloading (edit without restart) | COULD | ❌ | ✅ | ✅ |
| R10 | Version compatibility checking | SHOULD | ⚠️ | ✅ | ⚠️ |

### Non-Functional Requirements

| ID | Requirement | C++ | Python | Hybrid |
|----|-------------|-----|--------|--------|
| NR1 | Development time for users < 5 minutes | ❌ | ✅ | ✅ |
| NR2 | Learning curve suitable for beginners | ❌ | ✅ | ✅ |
| NR3 | Works with screen readers | ⚠️ | ✅ | ✅ |
| NR4 | Memory usage < 10MB per minigame | ✅ | ❌ | ⚠️ |
| NR5 | Startup time < 500ms | ✅ | ❌ | ⚠️ |

---

## Approach Comparison Matrix

### Technical Comparison

| Feature | C++ Dynamic Library | Python | Hybrid |
|---------|---------------------|--------|--------|
| **Plug-and-play** | ✅ Full | ✅ Full | ✅ Full |
| **No compilation** | ❌ Required | ✅ None needed | ✅ Python only |
| **Hot reload** | ❌ No | ✅ Yes | ✅ Python part |
| **Performance** | Native speed | Python interpreter speed | Depends on type |
| **Development speed** | Slow (compile-link-test) | Instant (edit-save-test) | Fast for Python |
| **Debugging** | C++ debugger required | Python debugger/prints | Both available |
| **Distribution** | Binary files (.so/.dll) | Source files (.py) | Both |
| **ABI compatibility** | ⚠️ Potential issues | ✅ No issues | ⚠️ C++ part only |
| **Dependencies** | None | pybind11, Python 3.6+ | Both optional |
| **Memory overhead** | ~100KB per minigame | ~1-5MB for interpreter + per minigame | Varies |
| **Startup time** | 10-50ms | 50-200ms | 50-250ms |
| **Cross-platform** | Requires platform-specific code | Native | Native |

### User Experience Comparison

| Aspect | C++ | Python | Hybrid |
|--------|-----|--------|--------|
| **Learning curve** | Steep (C++ knowledge) | Gentle (Python knowledge) | Medium |
| **Time to first minigame** | 30-60 minutes | 5-10 minutes | 5-10 minutes |
| **Iteration speed** | Slow (recompile) | Instant | Instant for Python |
| **Error messages** | Compiler errors | Python exceptions | Both |
| **Accessibility** | Keyboard-only, screen reader compatible with effort | Excellent screen reader support | Good |
| **Documentation needed** | Extensive | Minimal | Medium |

### Maintenance Comparison

| Aspect | C++ | Python | Hybrid |
|--------|-----|--------|--------|
| **Code complexity** | Medium | Low | High |
| **Build complexity** | Medium | Low | High |
| **Testing effort** | High | Medium | High |
| **Dependency management** | Simple (none) | Medium (Python versions) | Complex |
| **Version compatibility** | ⚠️ ABI breaks | ✅ Usually compatible | ⚠️ Partial |

---

## Detailed Architecture

### 1. C++ Dynamic Library Approach

#### Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                            NATHAN Game Engine (C++)                            │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐  │
│  │                         Engine                                       │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐            │  │
│  │  │ NodePool    │  │ EventBus    │  │  ResourceManager      │            │  │
│  │  └─────────────┘  └─────────────┘  │  - load(path)        │            │  │
│  │                                      │  - push_base_path()  │            │  │
│  │                                      │  - pop_base_path()   │            │  │
│  │                                      └─────────────────────┘            │  │
│  │                                                                     │  │
│  │  ┌─────────────────────────────────────────────────────────────┐   │  │
│  │  │                    MinigameLoader                             │   │  │
│  │  │  + scan(directory: string): vector<MinigameInfo>                │   │  │
│  │  │  + load(folder_path: string): MinigameHandle                     │   │  │
│  │  │  + unload(handle: MinigameHandle): void                         │   │  │
│  │  │  + create_scene(handle: MinigameHandle): unique_ptr<Node>       │   │  │
│  │  │  + get_available_minigames(): vector<MinigameInfo>             │   │  │
│  │  └─────────────────────────────────────────────────────────────┘   │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                            Minigames Directory                                  │
│                                                                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐                            │
│  │  puzzle/    │  │  racing/    │  │  memory/    │                            │
│  │             │  │             │  │             │                            │
│  │  ├── libpuzzle.so      │  │  ├── libracing.so│  │  ├── libmemory.so│           │
│  │  │  (Linux)            │  │  │  (Linux)      │  │  │  (Linux)      │           │
│  │  ├── puzzle.dll        │  │  ├── racing.dll  │  │  ├── memory.dll  │           │
│  │  │  (Windows)          │  │  │  (Windows)    │  │  │  (Windows)    │           │
│  │  ├── puzzle.dylib      │  │  ├── racing.dylib│  │  ├── memory.dylib│           │
│  │  │  (macOS)            │  │  │  (macOS)      │  │  │  (macOS)      │           │
│  │  ├── metadata.json     │  │  ├── metadata.json│  │  ├── metadata.json│           │
│  │  └── assets/           │  │  └── assets/      │  │  └── assets/      │           │
│  │      └── audio/        │  │      └── audio/   │  │      └── audio/   │           │
│  │          └── click.wav  │  │          └── vroom.wav│ │          └── beep.wav│           │
│  └─────────────┘  └─────────────┘  └─────────────┘                            │
└─────────────────────────────────────────────────────────────────────────────┘
```

#### Component Specifications

##### DynamicLibrary Class

**File**: `gameplay_foundations/minigame/dynamic_library.hpp`

```cpp
#pragma once

#include <string>
#include <memory>

namespace nathan {

class DynamicLibrary {
public:
    // Constructors
    DynamicLibrary() noexcept = default;
    explicit DynamicLibrary(std::string_view path);
    ~DynamicLibrary();
    
    // Rule of Five
    DynamicLibrary(const DynamicLibrary&) = delete;
    DynamicLibrary& operator=(const DynamicLibrary&) = delete;
    DynamicLibrary(DynamicLibrary&& other) noexcept;
    DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;
    
    // Status
    [[nodiscard]] bool is_loaded() const noexcept;
    [[nodiscard]] const std::string& get_error() const noexcept;
    [[nodiscard]] const std::string& get_path() const noexcept;
    
    // Symbol access
    template<typename T>
    [[nodiscard]] T get_symbol(std::string_view name) const;
    
    [[nodiscard]] void* get_symbol_raw(std::string_view name) const;
    
private:
    std::string path_;
    std::string error_;
    void* handle_ = nullptr;
    
    // Platform-specific implementations
    static void* load_library(const char* path, std::string& error);
    static void* get_symbol(void* handle, const char* name, std::string& error);
    static void free_library(void* handle);
};

} // namespace nathan
```

**File**: `gameplay_foundations/minigame/dynamic_library.cpp`

```cpp
#include "dynamic_library.hpp"

#include <system_error>
#include <stdexcept>

// Platform detection
#if defined(_WIN32)
    #include <windows.h>
    #define NATHAN_WINDOWS 1
#elif defined(__APPLE__)
    #include <dlfcn.h>
    #define NATHAN_MACOS 1
#else
    #include <dlfcn.h>
    #define NATHAN_LINUX 1
#endif

namespace nathan {

// ============================================================================
// Constructor / Destructor
// ============================================================================

DynamicLibrary::DynamicLibrary(std::string_view path) : path_(path) {
    handle_ = load_library(path_.c_str(), error_);
}

DynamicLibrary::~DynamicLibrary() {
    if (handle_) {
        free_library(handle_);
    }
}

DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept
    : path_(std::move(other.path_)),
      error_(std::move(other.error_)),
      handle_(other.handle_) {
    other.handle_ = nullptr;
}

DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept {
    if (this != &other) {
        // Free existing handle
        if (handle_) {
            free_library(handle_);
        }
        
        // Move data
        path_ = std::move(other.path_);
        error_ = std::move(other.error_);
        handle_ = other.handle_;
        other.handle_ = nullptr;
    }
    return *this;
}

// ============================================================================
// Status Methods
// ============================================================================

bool DynamicLibrary::is_loaded() const noexcept {
    return handle_ != nullptr;
}

const std::string& DynamicLibrary::get_error() const noexcept {
    return error_;
}

const std::string& DynamicLibrary::get_path() const noexcept {
    return path_;
}

// ============================================================================
// Symbol Access
// ============================================================================

template<typename T>
T DynamicLibrary::get_symbol(std::string_view name) const {
    void* sym = get_symbol_raw(name);
    if (!sym) {
        throw std::runtime_error("Symbol not found: " + std::string(name) + 
                                  " in library: " + path_ + 
                                  " - " + error_);
    }
    return reinterpret_cast<T>(sym);
}

void* DynamicLibrary::get_symbol_raw(std::string_view name) const {
    if (!handle_) {
        error_ = "Library not loaded";
        return nullptr;
    }
    
    std::string temp_error;
    void* sym = get_symbol(handle_, name.data(), temp_error);
    if (!sym) {
        error_ = temp_error;
        return nullptr;
    }
    return sym;
}

// ============================================================================
// Platform-Specific Implementations
// ============================================================================

#ifdef NATHAN_WINDOWS

void* DynamicLibrary::load_library(const char* path, std::string& error) {
    HMODULE handle = LoadLibraryA(path);
    if (!handle) {
        DWORD err = GetLastError();
        error = "LoadLibraryA failed with error code: " + std::to_string(err);
        return nullptr;
    }
    return handle;
}

void* DynamicLibrary::get_symbol(void* handle, const char* name, std::string& error) {
    FARPROC sym = GetProcAddress(static_cast<HMODULE>(handle), name);
    if (!sym) {
        DWORD err = GetLastError();
        error = "GetProcAddress failed with error code: " + std::to_string(err);
        return nullptr;
    }
    return reinterpret_cast<void*>(sym);
}

void DynamicLibrary::free_library(void* handle) {
    FreeLibrary(static_cast<HMODULE>(handle));
}

#elif defined(NATHAN_MACOS) || defined(NATHAN_LINUX)

void* DynamicLibrary::load_library(const char* path, std::string& error) {
    // RTLD_LAZY: Resolve symbols as needed
    // RTLD_GLOBAL: Symbols available for later dlopen calls
    void* handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        error = dlerror();
        if (error.empty()) {
            error = "dlopen failed with unknown error";
        }
        return nullptr;
    }
    return handle;
}

void* DynamicLibrary::get_symbol(void* handle, const char* name, std::string& error) {
    // Clear any existing error
    dlerror();
    
    void* sym = dlsym(handle, name);
    if (!sym) {
        error = dlerror();
        if (error.empty()) {
            error = "dlsym failed with unknown error";
        }
        return nullptr;
    }
    return sym;
}

void DynamicLibrary::free_library(void* handle) {
    dlclose(handle);
}

#endif

} // namespace nathan
```

##### Minigame Interface (C++)

Each C++ minigame must export these functions with C linkage:

```cpp
// Standard entry points - must be exported
#ifdef _WIN32
    #define MINIGAME_EXPORT __declspec(dllexport)
#else
    #define MINIGAME_EXPORT __attribute__((visibility("default")))
#endif

// Forward declaration
namespace nathan {
    class Node;
}

extern "C" {
    // Creates and returns the root scene node for this minigame
    // The caller (MinigameLoader) takes ownership of the returned pointer
    MINIGAME_EXPORT nathan::Node* minigame_create_scene();
    
    // Optional: cleanup function for the scene
    // If not provided, the loader will use 'delete'
    MINIGAME_EXPORT void minigame_destroy_scene(nathan::Node* scene);
    
    // Optional: returns metadata as JSON string
    // Format: {"name": "...", "description": "...", "version": "...", "author": "..."}
    MINIGAME_EXPORT const char* minigame_get_metadata();
    
    // Optional: returns the engine version this minigame was built for
    // Used for version compatibility checking
    MINIGAME_EXPORT const char* minigame_get_engine_version();
}
```

##### MinigameHandle Class

```cpp
// gameplay_foundations/minigame/minigame_handle.hpp
#pragma once

#include "dynamic_library.hpp"
#include <string>
#include <functional>
#include <memory>

namespace nathan {

class Node;

struct MinigameInfo {
    std::string name;
    std::string description;
    std::string version;
    std::string author;
    std::string folder_path;
    std::string library_path;
    bool is_valid = false;
};

class MinigameHandle {
public:
    std::string name;
    std::string folder_path;
    std::function<std::unique_ptr<Node>()> create_scene;
    std::function<void(Node*)> destroy_scene;
    DynamicLibrary library;
    
    MinigameHandle() = default;
    ~MinigameHandle();
    
    // Disable copying
    MinigameHandle(const MinigameHandle&) = delete;
    MinigameHandle& operator=(const MinigameHandle&) = delete;
    
    // Allow moving
    MinigameHandle(MinigameHandle&& other) noexcept;
    MinigameHandle& operator=(MinigameHandle&& other) noexcept;
    
    [[nodiscard]] bool is_valid() const noexcept;
    [[nodiscard]] bool is_loaded() const noexcept;
    
private:
    // For cleanup
    Node* last_created_scene_ = nullptr;
};

} // namespace nathan
```

```cpp
// gameplay_foundations/minigame/minigame_handle.cpp
#include "minigame_handle.hpp"
#include "node/node.hpp"

namespace nathan {

MinigameHandle::~MinigameHandle() {
    // Cleanup last created scene if it exists
    if (last_created_scene_) {
        if (destroy_scene) {
            destroy_scene(last_created_scene_);
        } else {
            delete last_created_scene_;
        }
        last_created_scene_ = nullptr;
    }
}

MinigameHandle::MinigameHandle(MinigameHandle&& other) noexcept
    : name(std::move(other.name)),
      folder_path(std::move(other.folder_path)),
      create_scene(std::move(other.create_scene)),
      destroy_scene(std::move(other.destroy_scene)),
      library(std::move(other.library)),
      last_created_scene_(other.last_created_scene_) {
    other.last_created_scene_ = nullptr;
}

MinigameHandle& MinigameHandle::operator=(MinigameHandle&& other) noexcept {
    if (this != &other) {
        // Cleanup current
        if (last_created_scene_) {
            if (destroy_scene) {
                destroy_scene(last_created_scene_);
            } else {
                delete last_created_scene_;
            }
        }
        
        // Move data
        name = std::move(other.name);
        folder_path = std::move(other.folder_path);
        create_scene = std::move(other.create_scene);
        destroy_scene = std::move(other.destroy_scene);
        library = std::move(other.library);
        last_created_scene_ = other.last_created_scene_;
        other.last_created_scene_ = nullptr;
    }
    return *this;
}

bool MinigameHandle::is_valid() const noexcept {
    return !name.empty() && library.is_loaded() && create_scene;
}

bool MinigameHandle::is_loaded() const noexcept {
    return library.is_loaded();
}

} // namespace nathan
```

##### MinigameLoader Class

```cpp
// gameplay_foundations/minigame/minigame_loader.hpp
#pragma once

#include "minigame_handle.hpp"
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace nathan {

class MinigameLoader {
public:
    MinigameLoader() = default;
    ~MinigameLoader() = default;
    
    // Delete copy/move (singleton-like access via Engine)
    MinigameLoader(const MinigameLoader&) = delete;
    MinigameLoader& operator=(const MinigameLoader&) = delete;
    MinigameLoader(MinigameLoader&&) = delete;
    MinigameLoader& operator=(MinigameLoader&&) = delete;
    
    // Scan a directory for available minigames
    [[nodiscard]] std::vector<MinigameInfo> scan(std::string_view directory = "minigames/");
    
    // Load a minigame by folder path
    [[nodiscard]] MinigameHandle load(std::string_view folder_path);
    
    // Unload a minigame
    void unload(MinigameHandle& handle);
    
    // Create a scene from a loaded minigame
    [[nodiscard]] std::unique_ptr<Node> create_scene(MinigameHandle& handle);
    
    // Get minigame by name
    [[nodiscard]] MinigameHandle load_by_name(std::string_view name);
    
    // Unload all loaded minigames
    void unload_all();
    
    // Get count of loaded minigames
    [[nodiscard]] size_t get_loaded_count() const;
    
private:
    using CreateSceneFunc = Node*(*)();
    using DestroySceneFunc = void(*)(Node*);
    using GetMetadataFunc = const char*(*)();
    using GetVersionFunc = const char*(*)();
    
    std::vector<MinigameHandle> loaded_minigames_;
    std::vector<MinigameInfo> cached_minigames_;
    
    // Helper methods
    [[nodiscard]] std::string get_library_path(const fs::path& folder) const;
    [[nodiscard]] MinigameInfo extract_metadata(const DynamicLibrary& lib, const fs::path& folder);
    [[nodiscard]] bool validate_minigame(const fs::path& folder) const;
    
    // Path resolution
    void push_minigame_base_path(const std::string& folder_path);
    void pop_minigame_base_path();
};

} // namespace nathan
```

```cpp
// gameplay_foundations/minigame/minigame_loader.cpp
#include "minigame_loader.hpp"
#include "engine/engine.hpp"
#include "resources/resource_manager.hpp"
#include <iostream>
#include <algorithm>

namespace nathan {

// ============================================================================
// Public Methods
// ============================================================================

std::vector<MinigameInfo> MinigameLoader::scan(std::string_view directory) {
    std::vector<MinigameInfo> results;
    
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "[MinigameLoader] Directory not found: " << directory << "\n";
        return results;
    }
    
    // Check cache first
    if (!cached_minigames_.empty()) {
        return cached_minigames_;
    }
    
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_directory()) {
            MinigameInfo info;
            info.folder_path = entry.path().string();
            
            // Get library path
            std::string lib_path = get_library_path(entry.path());
            if (lib_path.empty()) {
                continue; // No valid library file found
            }
            
            info.library_path = lib_path;
            info.name = entry.path().filename().string();
            
            // Try to load metadata from the library
            DynamicLibrary lib(lib_path);
            if (lib.is_loaded()) {
                info = extract_metadata(lib, entry.path());
            }
            
            // Ensure name is set
            if (info.name.empty()) {
                info.name = entry.path().filename().string();
            }
            
            info.is_valid = true;
            results.push_back(std::move(info));
        }
    }
    
    // Cache results
    cached_minigames_ = results;
    return results;
}

MinigameHandle MinigameLoader::load(std::string_view folder_path) {
    MinigameHandle handle;
    handle.folder_path = folder_path;
    
    // Get library path
    fs::path folder(folder_path);
    std::string lib_path = get_library_path(folder);
    
    if (lib_path.empty()) {
        std::cerr << "[MinigameLoader] No library file found in: " << folder_path << "\n";
        return handle;
    }
    
    // Load the library
    handle.library = DynamicLibrary(lib_path);
    if (!handle.library.is_loaded()) {
        std::cerr << "[MinigameLoader] Failed to load library: " << lib_path
                  << " - " << handle.library.get_error() << "\n";
        return handle;
    }
    
    // Get entry points
    try {
        auto create_func = handle.library.get_symbol<CreateSceneFunc>("minigame_create_scene");
        auto destroy_func = handle.library.get_symbol<DestroySceneFunc>("minigame_destroy_scene");
        auto metadata_func = handle.library.get_symbol<GetMetadataFunc>("minigame_get_metadata");
        auto version_func = handle.library.get_symbol<GetVersionFunc>("minigame_get_engine_version");
        
        handle.create_scene = [create_func, folder_path, this]() {
            Node* node = create_func();
            
            if (node && node->get_engine()) {
                // Push the minigame's base path for resource resolution
                node->get_engine()->get_resource_manager().push_base_path(folder_path);
            }
            
            // Store for cleanup
            // Note: We can't easily track this without modifying the handle
            // For now, the scene will manage its own resource cleanup
            
            return std::unique_ptr<Node>(node);
        };
        
        if (destroy_func) {
            handle.destroy_scene = destroy_func;
        } else {
            // Default to delete
            handle.destroy_scene = [](Node* node) { delete node; };
        }
        
        // Get metadata
        if (metadata_func) {
            const char* metadata = metadata_func();
            if (metadata) {
                // Parse JSON metadata (simplified for now)
                // In production: use nlohmann/json
                handle.name = folder.filename().string();
            }
        }
        
        if (handle.name.empty()) {
            handle.name = folder.filename().string();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[MinigameLoader] Failed to get symbols: " << e.what() << "\n";
        return MinigameHandle();
    }
    
    // Add to loaded list
    loaded_minigames_.push_back(handle);
    
    std::cout << "[MinigameLoader] Loaded minigame: " << handle.name
              << " from: " << folder_path << "\n";
    
    return handle;
}

void MinigameLoader::unload(MinigameHandle& handle) {
    if (handle.is_loaded()) {
        // Remove from loaded list
        auto it = std::remove_if(loaded_minigames_.begin(), loaded_minigames_.end(),
            [&](const MinigameHandle& h) { return h.folder_path == handle.folder_path; });
        if (it != loaded_minigames_.end()) {
            loaded_minigames_.erase(it, loaded_minigames_.end());
        }
        
        // Pop the base path if it was pushed
        // Note: This is handled by the ResourceManager's stack
    }
    
    // The handle will clean itself up when destroyed
}

std::unique_ptr<Node> MinigameLoader::create_scene(MinigameHandle& handle) {
    if (handle.create_scene) {
        return handle.create_scene();
    }
    std::cerr << "[MinigameLoader] No create_scene function for minigame: " << handle.name << "\n";
    return nullptr;
}

MinigameHandle MinigameLoader::load_by_name(std::string_view name) {
    // Find in cached list
    auto it = std::find_if(cached_minigames_.begin(), cached_minigames_.end(),
        [&](const MinigameInfo& info) { return info.name == name; });
    
    if (it != cached_minigames_.end()) {
        return load(it->folder_path);
    }
    
    std::cerr << "[MinigameLoader] Minigame not found: " << name << "\n";
    return MinigameHandle();
}

void MinigameLoader::unload_all() {
    for (auto& handle : loaded_minigames_) {
        // The handles will clean up when the vector is cleared
    }
    loaded_minigames_.clear();
    cached_minigames_.clear();
}

size_t MinigameLoader::get_loaded_count() const {
    return loaded_minigames_.size();
}

// ============================================================================
// Private Helper Methods
// ============================================================================

std::string MinigameLoader::get_library_path(const fs::path& folder) const {
    std::string name = folder.filename().string();
    
    // Try different naming conventions
    std::vector<fs::path> candidates = {
        folder / ("lib" + name + ".so"),      // Linux standard
        folder / (name + ".so"),               // Linux alternative
        folder / ("lib" + name + ".dylib"),   // macOS standard
        folder / (name + ".dylib"),           // macOS alternative
        folder / (name + ".dll"),              // Windows
        folder / (name + ".bundle"),          // macOS bundle
    };
    
    for (const auto& candidate : candidates) {
        if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
            return candidate.string();
        }
    }
    
    return "";
}

MinigameInfo MinigameLoader::extract_metadata(const DynamicLibrary& lib, const fs::path& folder) {
    MinigameInfo info;
    info.folder_path = folder.string();
    info.name = folder.filename().string();
    
    try {
        auto metadata_func = lib.get_symbol<GetMetadataFunc>("minigame_get_metadata");
        if (metadata_func) {
            const char* metadata_json = metadata_func();
            if (metadata_json) {
                // In production: parse JSON using nlohmann/json
                // For now, just extract basic info from the string
                info.is_valid = true;
                // Would parse name, description, version, author from JSON
            }
        }
    } catch (...) {
        // Ignore errors
    }
    
    return info;
}

bool MinigameLoader::validate_minigame(const fs::path& folder) const {
    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        return false;
    }
    
    return !get_library_path(folder).empty();
}

} // namespace nathan
```

##### Engine Integration

```cpp
// In gameplay_foundations/engine/engine.hpp
#include "minigame/minigame_loader.hpp"

class Engine {
public:
    // Existing methods...
    
    MinigameLoader& get_minigame_loader() { return minigame_loader_; }
    const MinigameLoader& get_minigame_loader() const { return minigame_loader_; }
    
    // Convenience method to load a minigame by name
    void load_minigame(std::string_view name);
    
private:
    // Existing members...
    MinigameLoader minigame_loader_;
};
```

```cpp
// In gameplay_foundations/engine/engine.cpp
#include "minigame/minigame_loader.hpp"

void Engine::load_minigame(std::string_view name) {
    auto handle = minigame_loader_.load_by_name(name);
    if (!handle.is_valid()) {
        std::cerr << "[Engine] Failed to load minigame: " << name << "\n";
        return;
    }
    
    auto scene = minigame_loader_.create_scene(handle);
    if (scene) {
        set_root(std::move(scene));
    } else {
        std::cerr << "[Engine] Failed to create scene for minigame: " << name << "\n";
    }
}
```

#### Usage in Game Code

```cpp
// In main menu scene
class MainMenu : public nathan::EventNode {
public:
    void setup() override {
        if (auto* engine = get_engine()) {
            // Scan for available minigames
            auto minigames = engine->get_minigame_loader().scan("minigames/");
            
            std::cout << "Available minigames (" << minigames.size() << "):\n";
            for (const auto& mg : minigames) {
                std::cout << "  - " << mg.name
                          << " (v" << mg.version << ")"
                          << "\n    " << mg.description << "\n";
            }
        }
    }
    
    void on_minigame_selected(std::string_view name) {
        if (auto* engine = get_engine()) {
            engine->load_minigame(name);
        }
    }
};
```

#### Example C++ Minigame

```cpp
// minigames/puzzle/puzzle_scene.hpp
#pragma once

#include "node/event_node.hpp"
#include "resources/audio_resource.hpp"

class PuzzleScene : public nathan::EventNode {
public:
    void setup() override;
    void loop(float delta) override;
    void cleanup() override;
    
private:
    std::shared_ptr<nathan::AudioResource> click_sound_;
    std::shared_ptr<nathan::AudioResource> background_music_;
    int score_ = 0;
};
```

```cpp
// minigames/puzzle/puzzle_scene.cpp
#include "puzzle_scene.hpp"
#include <iostream>

void PuzzleScene::setup() {
    std::cout << "PuzzleScene: Setup\n";
    
    if (auto* engine = get_engine()) {
        auto& rm = engine->get_resource_manager();
        
        // Load resources relative to minigame folder
        click_sound_ = std::dynamic_pointer_cast<nathan::AudioResource>(
            rm.load("assets/audio/click.wav"));
        background_music_ = std::dynamic_pointer_cast<nathan::AudioResource>(
            rm.load("assets/audio/background.mp3"));
        
        if (background_music_) {
            background_music_->play(-1); // Loop infinitely
            background_music_->set_volume(0.5f);
        }
    }
}

void PuzzleScene::loop(float delta) {
    // Game logic here
    score_ += static_cast<int>(delta * 10);
}

void PuzzleScene::cleanup() {
    std::cout << "PuzzleScene: Cleanup\n";
    
    if (background_music_) {
        background_music_->stop();
    }
}
```

```cpp
// minigames/puzzle/entry.cpp
#include "puzzle_scene.hpp"

// Platform-specific export macro
#ifdef _WIN32
    #define MINIGAME_EXPORT __declspec(dllexport)
#else
    #define MINIGAME_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {
    MINIGAME_EXPORT nathan::Node* minigame_create_scene() {
        return new PuzzleScene();
    }
    
    MINIGAME_EXPORT void minigame_destroy_scene(nathan::Node* scene) {
        delete scene;
    }
    
    MINIGAME_EXPORT const char* minigame_get_metadata() {
        return R"({"name": "Puzzle Game", "description": "Solve the puzzle to win", "version": "1.0.0", "author": "Your Name"}) ";
    }
    
    MINIGAME_EXPORT const char* minigame_get_engine_version() {
        return "1.0.0";
    }
}
```

#### CMakeLists.txt for C++ Minigame

```cmake
# minigames/puzzle/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Get minigame name from folder name
get_filename_component(MINIGAME_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Set library name based on platform
if(WIN32)
    set(LIB_NAME ${MINIGAME_NAME}.dll)
elseif(APPLE)
    set(LIB_NAME lib${MINIGAME_NAME}.dylib)
else()
    set(LIB_NAME lib${MINIGAME_NAME}.so)
endif()

# Create the shared library
add_library(${MINIGAME_NAME} MODULE
    entry.cpp
    puzzle_scene.cpp
)

# Set output name and directory
set_target_properties(${MINIGAME_NAME} PROPERTIES
    OUTPUT_NAME ${LIB_NAME}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    # For Windows: ensure DLL is built
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
)

# Link against engine and dependencies
target_link_libraries(${MINIGAME_NAME} PRIVATE
    gameplay_foundations
    SDL2::SDL2
    SDL2::Mixer
)

# Include directories
target_include_directories(${MINIGAME_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/gameplay_foundations
)

# Windows-specific: define export macro
if(WIN32)
    target_compile_definitions(${MINIGAME_NAME} PRIVATE MINIGAME_EXPORT=__declspec(dllexport))
endif()

# Install or copy to build directory
# Note: The LIBRARY_OUTPUT_DIRECTORY already puts it in the source dir
```

---

### 2. Python Approach

#### Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                            NATHAN Game Engine (C++)                            │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐  │
│  │                         Engine                                       │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐            │  │
│  │  │ NodePool    │  │ EventBus    │  │  ResourceManager      │            │  │
│  │  └─────────────┘  └─────────────┘  │  - load(path)        │            │  │
│  │                                      │  - push_base_path()  │            │  │
│  │                                      │  - pop_base_path()   │            │  │
│  │                                      └─────────────────────┘            │  │
│  │                                                                     │  │
│  │  ┌─────────────────────────────────────────────────────────────┐   │  │
│  │  │                    PythonMinigameLoader                        │   │  │
│  │  │  + initialize(): bool                                             │   │  │
│  │  │  + scan(directory: string): vector<MinigameInfo>                │   │  │
│  │  │  + load(folder_path: string): PythonMinigameHandle               │   │  │
│  │  │  + unload(handle: PythonMinigameHandle)                           │   │  │
│  │  │  + create_scene(handle: PythonMinigameHandle): unique_ptr<Node> │   │  │
│  │  └─────────────────────────────────────────────────────────────┘   │  │
│  │                                                                     │  │
│  │  ┌─────────────────────────────────────────────────────────────┐   │  │
│  │  │                    Python Bindings (pybind11)                    │   │  │
│  │  │  - Exposes C++ classes to Python                                  │   │  │
│  │  │  - Node, EventNode, Engine, ResourceManager, ConfigManager     │   │  │
│  │  │  - Resource, AudioResource                                        │   │  │
│  │  └─────────────────────────────────────────────────────────────┘   │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐  │
│  │                    Python Interpreter (Embedded)                     │  │
│  │  - pybind11::scoped_interpreter                                         │  │
│  │  - Managed by PythonMinigameLoader                                      │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                            Minigames Directory                                  │
│                                                                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐                            │
│  │  python_    │  │  python_    │  │  python_    │                            │
│  │  puzzle/    │  │  racing/    │  │  memory/    │                            │
│  │             │  │             │  │             │                            │
│  │  ├── main.py│  │  ├── main.py│  │  ├── main.py│                            │
│  │  ├── scene.py│  │  ├── scene.py│  │  ├── scene.py│                            │
│  │  └── assets/│  │  └── assets/│  │  └── assets/│                            │
│  │      └── audio/│  │      └── audio/│  │      └── audio/│                            │
│  │          └── click.wav│  │          └── vroom.wav│ │          └── beep.wav│                            │
│  └─────────────┘  └─────────────┘  └─────────────┘                            │
└─────────────────────────────────────────────────────────────────────────────┘
```

#### Component Specifications

##### Python Bindings

**File**: `gameplay_foundations/bindings/python_bindings.cpp`

```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "node/node.hpp"
#include "node/event_node.hpp"
#include "engine/engine.hpp"
#include "resources/resource_manager.hpp"
#include "resources/config_manager.hpp"
#include "resources/resource.hpp"
#include "resources/audio_resource.hpp"

namespace py = pybind11;

PYBIND11_MODULE(nathan, m) {
    // Enable automatic std::shared_ptr conversions
    py::module::import("pybind11_builtin");
    
    // ========================================================================
    // Node Hierarchy
    // ========================================================================
    
    py::class_<nathan::Node>(m, "Node")
        .def("setup", &nathan::Node::setup)
        .def("loop", &nathan::Node::loop)
        .def("cleanup", &nathan::Node::cleanup)
        .def("get_name", &nathan::Node::get_name)
        .def("set_name", &nathan::Node::set_name)
        .def("get_parent", &nathan::Node::get_parent, py::return_value_policy::reference)
        .def("get_engine", &nathan::Node::get_engine, py::return_value_policy::reference)
        .def("is_destroyed", &nathan::Node::is_destroyed)
        .def("set_destroyed", &nathan::Node::set_destroyed)
        .def("add_child", [](nathan::Node& self, py::object child_obj) {
            // Handle Python Node objects that wrap C++ Nodes
            if (py::isinstance<py::capsule>(child_obj)) {
                auto* child = py::cast<nathan::Node*>(child_obj);
                if (child) {
                    self.add_child(std::unique_ptr<nathan::Node>(child));
                    return;
                }
            }
            throw py::type_error("Child must be a Node or derived class");
        })
        .def("remove_child", [](nathan::Node& self, py::object child_obj) {
            auto* child = py::cast<nathan::Node*>(child_obj);
            if (child) {
                self.remove_child(child);
            }
        })
        .def("destroy", &nathan::Node::destroy);
    
    // EventNode
    py::class_<nathan::EventNode, nathan::Node>(m, "EventNode")
        .def(py::init<>())
        .def("on", [](nathan::EventNode& self, const std::string& event_type, py::function callback) {
            // Store the callback and create a C++ lambda
            // This is simplified - actual implementation needs to handle lifetime
            self.on(event_type, [callback](auto&&... args) {
                callback(args...);
            });
        })
        .def("once", [](nathan::EventNode& self, const std::string& event_type, py::function callback) {
            self.once(event_type, [callback](auto&&... args) {
                callback(args...);
            });
        })
        .def("off", [](nathan::EventNode& self, const std::string& event_type) {
            self.off(event_type);
        })
        .def("off_all", &nathan::EventNode::off_all);
    
    // ========================================================================
    // Engine
    // ========================================================================
    
    py::class_<nathan::Engine>(m, "Engine")
        .def("run", &nathan::Engine::run)
        .def("stop", &nathan::Engine::stop)
        .def("set_root", [](nathan::Engine& self, py::object node_obj) {
            auto* node = py::cast<nathan::Node*>(node_obj);
            if (node) {
                self.set_root(std::unique_ptr<nathan::Node>(node));
            }
        })
        .def("get_root", &nathan::Engine::get_root, py::return_value_policy::reference)
        .def("get_node_pool", &nathan::Engine::get_node_pool, py::return_value_policy::reference)
        .def("get_event_bus", &nathan::Engine::get_event_bus, py::return_value_policy::reference)
        .def("get_resource_manager", &nathan::Engine::get_resource_manager, py::return_value_policy::reference)
        .def("get_config_manager", &nathan::Engine::get_config_manager, py::return_value_policy::reference)
        .def("get_minigame_loader", &nathan::Engine::get_minigame_loader, py::return_value_policy::reference);
    
    // ========================================================================
    // Resource System
    // ========================================================================
    
    // Resource base class
    py::class_<nathan::Resource>(m, "Resource")
        .def("get_path", &nathan::Resource::get_path)
        .def("get_type", &nathan::Resource::get_type);
    
    // AudioResource
    py::class_<nathan::AudioResource, nathan::Resource>(m, "AudioResource")
        .def("play", [](nathan::AudioResource& self, int loops) {
            self.play(loops);
        }, py::arg("loops") = 0)
        .def("stop", &nathan::AudioResource::stop)
        .def("set_volume", &nathan::AudioResource::set_volume)
        .def("get_volume", &nathan::AudioResource::get_volume)
        .def("is_loaded", &nathan::AudioResource::is_loaded);
    
    // ResourceManager
    py::class_<nathan::ResourceManager>(m, "ResourceManager")
        .def("load", [](nathan::ResourceManager& self, const std::string& path) {
            return self.load(path);
        })
        .def("get", [](nathan::ResourceManager& self, const std::string& path) {
            return self.get(path);
        })
        .def("is_loaded", &nathan::ResourceManager::is_loaded)
        .def("unload", &nathan::ResourceManager::unload)
        .def("unload_unused", &nathan::ResourceManager::unload_unused)
        .def("unload_all", &nathan::ResourceManager::unload_all)
        .def("push_base_path", &nathan::ResourceManager::push_base_path)
        .def("pop_base_path", &nathan::ResourceManager::pop_base_path)
        .def("get_loaded_count", &nathan::ResourceManager::get_loaded_count)
        .def("get_total_count", &nathan::ResourceManager::get_total_count);
    
    // ========================================================================
    // ConfigManager
    // ========================================================================
    
    py::class_<nathan::ConfigManager>(m, "ConfigManager")
        .def("load", [](nathan::ConfigManager& self, const std::string& path) {
            return self.load(path);
        })
        .def("save", [](nathan::ConfigManager& self, const std::string& path) {
            return self.save(path);
        })
        .def("get_float", &nathan::ConfigManager::get_float)
        .def("set_float", &nathan::ConfigManager::set_float)
        .def("get_int", &nathan::ConfigManager::get_int)
        .def("set_int", &nathan::ConfigManager::set_int)
        .def("get_bool", &nathan::ConfigManager::get_bool)
        .def("set_bool", &nathan::ConfigManager::set_bool)
        .def("get_string", &nathan::ConfigManager::get_string)
        .def("set_string", &nathan::ConfigManager::set_string)
        .def("get_master_volume", &nathan::ConfigManager::get_master_volume)
        .def("set_master_volume", &nathan::ConfigManager::set_master_volume)
        .def("get_music_volume", &nathan::ConfigManager::get_music_volume)
        .def("set_music_volume", &nathan::ConfigManager::set_music_volume)
        .def("get_sfx_volume", &nathan::ConfigManager::get_sfx_volume)
        .def("set_sfx_volume", &nathan::ConfigManager::set_sfx_volume)
        .def("is_dirty", &nathan::ConfigManager::is_dirty)
        .def("save_if_dirty", &nathan::ConfigManager::save_if_dirty);
    
    // ========================================================================
    // Minigame Loader (for Python minigames)
    // ========================================================================
    
    py::class_<nathan::PythonMinigameLoader>(m, "PythonMinigameLoader")
        .def("initialize", &nathan::PythonMinigameLoader::initialize)
        .def("scan", &nathan::PythonMinigameLoader::scan)
        .def("load", &nathan::PythonMinigameLoader::load)
        .def("unload", &nathan::PythonMinigameLoader::unload);
    
    // ========================================================================
    // Version Info
    // ========================================================================
    
    m.attr("ENGINE_VERSION") = py::make_tuple(1, 0, 0);
    m.attr("__version__") = "1.0.0";
}
```

##### PythonMinigameLoader Class

**File**: `gameplay_foundations/minigame/python_minigame_loader.hpp`

```cpp
#pragma once

#include <pybind11/embed.h>
#include "node/node.hpp"
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace py = pybind11;

namespace nathan {

class Node;

struct MinigameInfo;

class PythonMinigameHandle {
public:
    std::string name;
    std::string folder_path;
    std::function<std::unique_ptr<Node>()> create_scene;
    py::module python_module;
    
    PythonMinigameHandle() = default;
    ~PythonMinigameHandle();
    
    PythonMinigameHandle(const PythonMinigameHandle&) = delete;
    PythonMinigameHandle& operator=(const PythonMinigameHandle&) = delete;
    PythonMinigameHandle(PythonMinigameHandle&& other) noexcept;
    PythonMinigameHandle& operator=(PythonMinigameHandle&& other) noexcept;
    
    [[nodiscard]] bool is_valid() const noexcept;
};

class PythonMinigameLoader {
public:
    PythonMinigameLoader();
    ~PythonMinigameLoader();
    
    // Initialize Python interpreter
    bool initialize();
    bool is_initialized() const { return python_initialized_; }
    
    // Scan directory for Python minigames
    [[nodiscard]] std::vector<MinigameInfo> scan(std::string_view directory = "minigames/");
    
    // Load a Python minigame
    [[nodiscard]] PythonMinigameHandle load(std::string_view folder_path);
    
    // Unload a Python minigame
    void unload(PythonMinigameHandle& handle);
    
    // Create scene from loaded handle
    [[nodiscard]] std::unique_ptr<Node> create_scene(PythonMinigameHandle& handle);
    
    // Unload all
    void unload_all();
    
private:
    bool python_initialized_ = false;
    std::unique_ptr<py::scoped_interpreter> interpreter_;
    std::vector<PythonMinigameHandle> loaded_minigames_;
    
    // Helper methods
    [[nodiscard]] bool check_for_main_py(const std::string& folder) const;
    [[nodiscard]] std::string extract_name(const std::string& folder) const;
};

} // namespace nathan
```

**File**: `gameplay_foundations/minigame/python_minigame_loader.cpp`

```cpp
#include "python_minigame_loader.hpp"
#include "engine/engine.hpp"
#include "resources/resource_manager.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace nathan {

// ============================================================================
// PythonMinigameHandle
// ============================================================================

PythonMinigameHandle::~PythonMinigameHandle() {
    // Cleanup Python module reference
    if (!python_module.is_none()) {
        python_module = py::none();
    }
}

PythonMinigameHandle::PythonMinigameHandle(PythonMinigameHandle&& other) noexcept
    : name(std::move(other.name)),
      folder_path(std::move(other.folder_path)),
      create_scene(std::move(other.create_scene)),
      python_module(std::move(other.python_module)) {
    other.python_module = py::none();
}

PythonMinigameHandle& PythonMinigameHandle::operator=(PythonMinigameHandle&& other) noexcept {
    if (this != &other) {
        if (!python_module.is_none()) {
            python_module = py::none();
        }
        
        name = std::move(other.name);
        folder_path = std::move(other.folder_path);
        create_scene = std::move(other.create_scene);
        python_module = std::move(other.python_module);
        other.python_module = py::none();
    }
    return *this;
}

bool PythonMinigameHandle::is_valid() const noexcept {
    return !name.empty() && !python_module.is_none() && create_scene;
}

// ============================================================================
// PythonMinigameLoader
// ============================================================================

PythonMinigameLoader::PythonMinigameLoader() {
    // Interpreter will be initialized on first use
}

PythonMinigameLoader::~PythonMinigameLoader() {
    unload_all();
    interpreter_.reset();
}

bool PythonMinigameLoader::initialize() {
    if (python_initialized_) {
        return true;
    }
    
    try {
        // Initialize Python interpreter
        interpreter_ = std::make_unique<py::scoped_interpreter>();
        python_initialized_ = true;
        
        // Import the nathan module to make it available
        py::module::import("nathan");
        
        std::cout << "[PythonMinigameLoader] Python interpreter initialized\n";
        return true;
    } catch (const py::error_already_set& e) {
        std::cerr << "[PythonMinigameLoader] Failed to initialize Python: " << e.what() << "\n";
        python_initialized_ = false;
        interpreter_.reset();
        return false;
    } catch (const std::exception& e) {
        std::cerr << "[PythonMinigameLoader] Failed to initialize Python: " << e.what() << "\n";
        python_initialized_ = false;
        interpreter_.reset();
        return false;
    }
}

std::vector<MinigameInfo> PythonMinigameLoader::scan(std::string_view directory) {
    std::vector<MinigameInfo> results;
    
    if (!python_initialized_ && !initialize()) {
        return results;
    }
    
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "[PythonMinigameLoader] Directory not found: " << directory << "\n";
        return results;
    }
    
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_directory() && check_for_main_py(entry.path().string())) {
            MinigameInfo info;
            info.folder_path = entry.path().string();
            info.name = extract_name(entry.path().string());
            info.description = "Python minigame";
            info.version = "1.0.0";
            info.is_valid = true;
            results.push_back(std::move(info));
        }
    }
    
    return results;
}

PythonMinigameHandle PythonMinigameLoader::load(std::string_view folder_path) {
    PythonMinigameHandle handle;
    handle.folder_path = folder_path;
    
    if (!python_initialized_ && !initialize()) {
        return handle;
    }
    
    try {
        // Add minigame folder to Python path
        std::string setup_code = "import sys; sys.path.insert(0, '" + 
                                std::string(folder_path) + "')";
        py::exec(setup_code);
        
        // Import the main module
        py::module main_module = py::module::import("main");
        
        // Get the create_scene function
        py::function create_func = main_module.attr("create_scene");
        
        handle.python_module = main_module;
        handle.name = extract_name(std::string(folder_path));
        
        // Create the scene creation function
        handle.create_scene = [create_func, folder_path]() {
            py::object scene_obj = create_func();
            
            // Extract C++ Node* from Python object
            // The Python object should be a wrapper around a C++ Node*
            nathan::Node* node = py::cast<nathan::Node*>(scene_obj);
            
            if (node && node->get_engine()) {
                // Push the minigame's base path for resource resolution
                node->get_engine()->get_resource_manager().push_base_path(folder_path);
            }
            
            return std::unique_ptr<Node>(node);
        };
        
        loaded_minigames_.push_back(handle);
        std::cout << "[PythonMinigameLoader] Loaded Python minigame: " << handle.name
                  << " from: " << folder_path << "\n";
        
    } catch (const py::error_already_set& e) {
        std::cerr << "[PythonMinigameLoader] Python error loading minigame: " 
                  << e.what() << "\n";
        // Try to clean up
        try {
            std::string cleanup_code = "import sys; sys.path.remove('" + 
                                     std::string(folder_path) + "')";
            py::exec(cleanup_code);
        } catch (...) {
            // Ignore cleanup errors
        }
    } catch (const std::exception& e) {
        std::cerr << "[PythonMinigameLoader] Failed to load minigame: " 
                  << e.what() << "\n";
    }
    
    return handle;
}

void PythonMinigameLoader::unload(PythonMinigameHandle& handle) {
    if (!handle.python_module.is_none()) {
        try {
            // Remove from sys.path
            std::string cleanup_code = "import sys; sys.path.remove('" + 
                                     handle.folder_path + "')";
            py::exec(cleanup_code);
        } catch (...) {
            // Ignore errors during cleanup
        }
        
        handle.python_module = py::none();
        handle.create_scene = nullptr;
    }
    
    // Remove from loaded list
    auto it = std::remove_if(loaded_minigames_.begin(), loaded_minigames_.end(),
        [&](const PythonMinigameHandle& h) {
            return h.folder_path == handle.folder_path;
        });
    if (it != loaded_minigames_.end()) {
        loaded_minigames_.erase(it, loaded_minigames_.end());
    }
}

std::unique_ptr<Node> PythonMinigameLoader::create_scene(PythonMinigameHandle& handle) {
    if (handle.create_scene) {
        return handle.create_scene();
    }
    std::cerr << "[PythonMinigameLoader] No create_scene function for: " << handle.name << "\n";
    return nullptr;
}

void PythonMinigameLoader::unload_all() {
    for (auto& handle : loaded_minigames_) {
        unload(handle);
    }
    loaded_minigames_.clear();
}

// ============================================================================
// Private Helper Methods
// ============================================================================

bool PythonMinigameLoader::check_for_main_py(const std::string& folder) const {
    fs::path main_py = fs::path(folder) / "main.py";
    return fs::exists(main_py) && fs::is_regular_file(main_py);
}

std::string PythonMinigameLoader::extract_name(const std::string& folder) const {
    fs::path p(folder);
    return p.filename().string();
}

} // namespace nathan
```

##### Engine Integration for Python

```cpp
// In gameplay_foundations/engine/engine.hpp
#include "minigame/python_minigame_loader.hpp"

class Engine {
public:
    // Existing methods...
    
    PythonMinigameLoader& get_python_minigame_loader() { 
        return python_minigame_loader_; 
    }
    const PythonMinigameLoader& get_python_minigame_loader() const { 
        return python_minigame_loader_; 
    }
    
    // Convenience method
    void load_python_minigame(std::string_view name);
    
private:
    // Existing members...
    PythonMinigameLoader python_minigame_loader_;
};
```

```cpp
// In gameplay_foundations/engine/engine.cpp
#include "minigame/python_minigame_loader.hpp"

void Engine::load_python_minigame(std::string_view name) {
    auto handle = python_minigame_loader_.load(name);
    if (!handle.is_valid()) {
        std::cerr << "[Engine] Failed to load Python minigame: " << name << "\n";
        return;
    }
    
    auto scene = python_minigame_loader_.create_scene(handle);
    if (scene) {
        set_root(std::move(scene));
    } else {
        std::cerr << "[Engine] Failed to create scene for Python minigame: " << name << "\n";
    }
}
```

#### Python Minigame Structure

```
minigames/python_puzzle/
├── main.py              # Entry point - creates the scene
├── scene.py             # Scene definition
├── player.py            # Player node
└── assets/
    └── audio/
        └── click.wav
```

```python
# minigames/python_puzzle/main.py
import nathan
from scene import GameScene

def create_scene():
    """
    Factory function called by C++ PythonMinigameLoader.
    Must return a Node or EventNode instance.
    """
    return GameScene()
```

```python
# minigames/python_puzzle/scene.py
import nathan
from player import PlayerNode

class GameScene(nathan.EventNode):
    def __init__(self):
        super().__init__()
        self.click_sound = None
        self.player = None
    
    def setup(self):
        """Called when the scene is created."""
        print("Python GameScene: Setup")
        
        engine = self.get_engine()
        if engine:
            # Access resource manager
            rm = engine.get_resource_manager()
            
            # Load resources relative to minigame folder
            self.click_sound = rm.load("assets/audio/click.wav")
            
            # Create player node
            self.player = PlayerNode()
            self.player.set_name("python_player")
            self.add_child(self.player)
            
            # Set up event listeners
            self.on("score_update", self.on_score_update)
    
    def loop(self, delta):
        """Called every frame. delta = time since last frame in seconds."""
        # Game logic
        pass
    
    def cleanup(self):
        """Called when the scene is destroyed."""
        print("Python GameScene: Cleanup")
        self.click_sound = None
        self.player = None
    
    def on_score_update(self, event):
        """Event handler."""
        print(f"Score updated: {event}")
        if self.click_sound:
            self.click_sound.play()
```

```python
# minigames/python_puzzle/player.py
import nathan

class PlayerNode(nathan.EventNode):
    def __init__(self):
        super().__init__()
        self.x = 0.0
        self.y = 0.0
        self.speed = 5.0
        self.jump_sound = None
    
    def setup(self):
        """Called when the node is created."""
        print("PlayerNode: Setup")
        
        engine = self.get_engine()
        if engine:
            rm = engine.get_resource_manager()
            self.jump_sound = rm.load("assets/audio/jump.wav")
    
    def loop(self, delta):
        """Called every frame."""
        self.x += delta * self.speed
        # print(f"Player at x={self.x}")
    
    def jump(self):
        """Custom method."""
        print("Player jumped!")
        if self.jump_sound:
            self.jump_sound.play()
        
        # Emit event
        self.emit("player_jumped", {"x": self.x, "y": self.y})
```

#### CMakeLists.txt for Python Support

```cmake
# Main CMakeLists.txt additions

# Find pybind11 and Python
find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)

# Check if Python is available
if(pybind11_FOUND AND Python_FOUND)
    message(STATUS "Python support enabled")
    
    # Python bindings library
    add_library(nathan_python MODULE
        gameplay_foundations/bindings/python_bindings.cpp
    )
    
    target_link_libraries(nathan_python PRIVATE
        gameplay_foundations
        pybind11::embed
        ${Python_LIBRARIES}
    )
    
    target_include_directories(nathan_python PRIVATE
        ${pybind11_INCLUDE_DIRS}
        ${Python_INCLUDE_DIRS}
    )
    
    # Set output properties
    set_target_properties(nathan_python PROPERTIES
        OUTPUT_NAME "nathan"
        SUFFIX ".so"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    # Python minigame loader
    add_library(python_minigame_loader
        gameplay_foundations/minigame/python_minigame_loader.cpp
    )
    
    target_link_libraries(python_minigame_loader PRIVATE
        gameplay_foundations
        nathan_python
        pybind11::embed
        ${Python_LIBRARIES}
    )
    
    # Link Python loader to main library
    target_link_libraries(gameplay_foundations PRIVATE python_minigame_loader)
    
    # Link to demo
    target_link_libraries(demo_game PRIVATE nathan_python)
    
else()
    message(STATUS "Python support disabled - pybind11 or Python not found")
endif()
```

---

### 3. Hybrid Approach

The hybrid approach combines both C++ and Python loaders into a unified interface.

#### Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                            NATHAN Game Engine (C++)                            │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐  │
│  │                         Engine                                       │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐            │  │
│  │  │ NodePool    │  │ EventBus    │  │  ResourceManager      │            │  │
│  │  └─────────────┘  └─────────────┘  └─────────────────────┘            │  │
│  │                                                                     │  │
│  │  ┌─────────────────────────────────────────────────────────────┐   │  │
│  │  │                    MinigameLoader                              │   │  │
│  │  │  + scan(directory: string): vector<MinigameInfo>                │   │  │
│  │  │  + load(folder_path: string): MinigameHandle                     │   │  │
│  │  │  + load_by_name(name: string): MinigameHandle                   │   │  │
│  │  │  + unload(handle: MinigameHandle)                               │   │  │
│  │  │  + create_scene(handle: MinigameHandle): unique_ptr<Node>       │   │  │
│  │  └─────────────────────────────────────────────────────────────┘   │  │
│  │           │                                              │                │  │
│  │           ▼                                              ▼                │  │
│  │  ┌─────────────────┐                        ┌─────────────────┐    │  │
│  │  │ CppMinigameLoader │                        │PythonMinigameLoader│   │  │
│  │  │ + scan()        │                        │  + scan()        │   │  │
│  │  │ + load()        │                        │  + load()        │   │  │
│  │  │ + create_scene()│                        │  + create_scene()│   │  │
│  │  └─────────────────┘                        └─────────────────┘    │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────────┘
                              │
              ┌───────────────────────┬───────────────────────┐
              ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│  C++ Minigames  │    │ Python Minigames│    │  Hybrid Games   │
│  (.so/.dll)     │    │   (.py files)   │    │  (Mix of both)  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

#### MinigameType Enum

```cpp
// gameplay_foundations/minigame/minigame_types.hpp
#pragma once

namespace nathan {

enum class MinigameType {
    Unknown,
    Cpp,
    Python
};

} // namespace nathan
```

#### Unified MinigameHandle

```cpp
// gameplay_foundations/minigame/unified_minigame_handle.hpp
#pragma once

#include "minigame_types.hpp"
#include "minigame_handle.hpp"  // C++ handle
#include "python_minigame_loader.hpp"  // For Python handle
#include <variant>

namespace nathan {

// Forward declarations
class CppMinigameHandle;

class UnifiedMinigameHandle {
public:
    std::string name;
    std::string folder_path;
    MinigameType type;
    std::function<std::unique_ptr<Node>()> create_scene;
    
    // Type-specific handles
    MinigameHandle cpp_handle;      // Valid if type == Cpp
    PythonMinigameHandle python_handle;  // Valid if type == Python
    
    UnifiedMinigameHandle() : type(MinigameType::Unknown) {}
    ~UnifiedMinigameHandle();
    
    UnifiedMinigameHandle(const UnifiedMinigameHandle&) = delete;
    UnifiedMinigameHandle& operator=(const UnifiedMinigameHandle&) = delete;
    UnifiedMinigameHandle(UnifiedMinigameHandle&& other) noexcept;
    UnifiedMinigameHandle& operator=(UnifiedMinigameHandle&& other) noexcept;
    
    [[nodiscard]] bool is_valid() const noexcept;
    [[nodiscard]] MinigameType get_type() const noexcept;
};

} // namespace nathan
```

#### Unified MinigameLoader

```cpp
// gameplay_foundations/minigame/minigame_loader.hpp
#pragma once

#include "unified_minigame_handle.hpp"
#include "minigame_handle.hpp"
#include "python_minigame_loader.hpp"
#include <string>
#include <vector>

namespace nathan {

struct MinigameInfo;

class MinigameLoader {
public:
    MinigameLoader();
    ~MinigameLoader();
    
    // Delete copy/move
    MinigameLoader(const MinigameLoader&) = delete;
    MinigameLoader& operator=(const MinigameLoader&) = delete;
    
    // Scan directory for all minigames (both C++ and Python)
    [[nodiscard]] std::vector<MinigameInfo> scan(std::string_view directory = "minigames/");
    
    // Load a minigame by folder path
    [[nodiscard]] UnifiedMinigameHandle load(std::string_view folder_path);
    
    // Load a minigame by name
    [[nodiscard]] UnifiedMinigameHandle load_by_name(std::string_view name);
    
    // Unload a minigame
    void unload(UnifiedMinigameHandle& handle);
    
    // Create scene from loaded handle
    [[nodiscard]] std::unique_ptr<Node> create_scene(UnifiedMinigameHandle& handle);
    
    // Unload all
    void unload_all();
    
    // Get loaded count
    [[nodiscard]] size_t get_loaded_count() const;
    
    // Check if Python is available
    [[nodiscard]] bool is_python_available() const;
    
private:
    bool python_available_ = false;
    CppMinigameLoader cpp_loader_;
    PythonMinigameLoader python_loader_;
    std::vector<UnifiedMinigameHandle> loaded_minigames_;
    std::vector<MinigameInfo> cached_minigames_;
    
    // Helper methods
    [[nodiscard]] bool is_directory(const std::string& path) const;
    [[nodiscard]] bool has_cpp_library(const std::string& folder) const;
    [[nodiscard]] bool has_python_main(const std::string& folder) const;
};

} // namespace nathan
```

```cpp
// gameplay_foundations/minigame/minigame_loader.cpp
#include "minigame_loader.hpp"
#include "minigame_handle.hpp"
#include "python_minigame_loader.hpp"
#include "engine/engine.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace nathan {

// ============================================================================
// Constructor / Destructor
// ============================================================================

MinigameLoader::MinigameLoader() {
    // Initialize Python loader
    python_available_ = python_loader_.initialize();
    if (python_available_) {
        std::cout << "[MinigameLoader] Python support enabled\n";
    } else {
        std::cout << "[MinigameLoader] Python support disabled (pybind11/Python not available)\n";
    }
}

MinigameLoader::~MinigameLoader() {
    unload_all();
}

// ============================================================================
// Public Methods
// ============================================================================

std::vector<MinigameInfo> MinigameLoader::scan(std::string_view directory) {
    std::vector<MinigameInfo> results;
    
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "[MinigameLoader] Directory not found: " << directory << "\n";
        return results;
    }
    
    // Check cache
    if (!cached_minigames_.empty()) {
        return cached_minigames_;
    }
    
    // Scan for C++ minigames
    auto cpp_minigames = cpp_loader_.scan(directory);
    results.insert(results.end(), cpp_minigames.begin(), cpp_minigames.end());
    
    // Scan for Python minigames (if available)
    if (python_available_) {
        auto python_minigames = python_loader_.scan(directory);
        results.insert(results.end(), python_minigames.begin(), python_minigames.end());
    }
    
    // Cache results
    cached_minigames_ = results;
    return results;
}

UnifiedMinigameHandle MinigameLoader::load(std::string_view folder_path) {
    UnifiedMinigameHandle handle;
    handle.folder_path = folder_path;
    
    // Try Python first (if available and has main.py)
    if (python_available_ && has_python_main(std::string(folder_path))) {
        auto python_handle = python_loader_.load(folder_path);
        if (python_handle.is_valid()) {
            handle.type = MinigameType::Python;
            handle.name = python_handle.name;
            handle.python_handle = std::move(python_handle);
            handle.create_scene = [this, folder_path]() {
                auto temp_handle = python_loader_.load(folder_path);
                return python_loader_.create_scene(temp_handle);
            };
            loaded_minigames_.push_back(handle);
            return handle;
        }
    }
    
    // Try C++
    auto cpp_handle = cpp_loader_.load(folder_path);
    if (cpp_handle.is_valid()) {
        handle.type = MinigameType::Cpp;
        handle.name = cpp_handle.name;
        handle.cpp_handle = std::move(cpp_handle);
        handle.create_scene = [this, folder_path]() {
            auto temp_handle = cpp_loader_.load(folder_path);
            return cpp_loader_.create_scene(temp_handle);
        };
        loaded_minigames_.push_back(handle);
        return handle;
    }
    
    std::cerr << "[MinigameLoader] Failed to load minigame from: " << folder_path << "\n";
    return UnifiedMinigameHandle();
}

UnifiedMinigameHandle MinigameLoader::load_by_name(std::string_view name) {
    auto minigames = scan();
    for (const auto& info : minigames) {
        if (info.name == name) {
            return load(info.folder_path);
        }
    }
    std::cerr << "[MinigameLoader] Minigame not found: " << name << "\n";
    return UnifiedMinigameHandle();
}

void MinigameLoader::unload(UnifiedMinigameHandle& handle) {
    switch (handle.type) {
        case MinigameType::Cpp:
            cpp_loader_.unload(handle.cpp_handle);
            break;
        case MinigameType::Python:
            if (python_available_) {
                python_loader_.unload(handle.python_handle);
            }
            break;
        case MinigameType::Unknown:
            break;
    }
    
    // Remove from loaded list
    auto it = std::remove_if(loaded_minigames_.begin(), loaded_minigames_.end(),
        [&](const UnifiedMinigameHandle& h) {
            return h.folder_path == handle.folder_path;
        });
    if (it != loaded_minigames_.end()) {
        loaded_minigames_.erase(it, loaded_minigames_.end());
    }
}

std::unique_ptr<Node> MinigameLoader::create_scene(UnifiedMinigameHandle& handle) {
    if (handle.create_scene) {
        return handle.create_scene();
    }
    std::cerr << "[MinigameLoader] No create_scene function for: " << handle.name << "\n";
    return nullptr;
}

void MinigameLoader::unload_all() {
    for (auto& handle : loaded_minigames_) {
        unload(handle);
    }
    loaded_minigames_.clear();
    cached_minigames_.clear();
}

size_t MinigameLoader::get_loaded_count() const {
    return loaded_minigames_.size();
}

bool MinigameLoader::is_python_available() const {
    return python_available_;
}

// ============================================================================
// Private Helper Methods
// ============================================================================

bool MinigameLoader::is_directory(const std::string& path) const {
    return fs::is_directory(path);
}

bool MinigameLoader::has_cpp_library(const std::string& folder) const {
    return cpp_loader_.get_library_path(folder).empty();
}

bool MinigameLoader::has_python_main(const std::string& folder) const {
    fs::path main_py = fs::path(folder) / "main.py";
    return fs::exists(main_py) && fs::is_regular_file(main_py);
}

} // namespace nathan
```

#### Engine Integration for Hybrid

```cpp
// In gameplay_foundations/engine/engine.hpp
#include "minigame/minigame_loader.hpp"

class Engine {
public:
    // Existing methods...
    
    MinigameLoader& get_minigame_loader() { return minigame_loader_; }
    const MinigameLoader& get_minigame_loader() const { return minigame_loader_; }
    
    // Convenience method
    void load_minigame(std::string_view name);
    
private:
    // Existing members...
    MinigameLoader minigame_loader_;
};
```

```cpp
// In gameplay_foundations/engine/engine.cpp
#include "minigame/minigame_loader.hpp"

void Engine::load_minigame(std::string_view name) {
    auto handle = minigame_loader_.load_by_name(name);
    if (!handle.is_valid()) {
        std::cerr << "[Engine] Failed to load minigame: " << name << "\n";
        return;
    }
    
    auto scene = minigame_loader_.create_scene(handle);
    if (scene) {
        set_root(std::move(scene));
    } else {
        std::cerr << "[Engine] Failed to create scene for minigame: " << name << "\n";
    }
}
```

---

## Class Diagrams

### C++ Dynamic Library Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                Engine                                        │
├─────────────────────────────────────────────────────────────────────────────┤
│ - root_: Node*                                                               │
│ - node_pool_: NodePool                                                       │
│ - event_bus_: EventBus                                                        │
│ - resource_manager_: ResourceManager                                         │
│ - config_manager_: ConfigManager                                             │
│ + get_minigame_loader(): MinigameLoader&                                     │
│ + load_minigame(name: string)                                                │
└─────────────────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                            MinigameLoader                                     │
├─────────────────────────────────────────────────────────────────────────────┤
│ - loaded_minigames_: vector<MinigameHandle>                                  │
│ - cached_minigames_: vector<MinigameInfo>                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│ + scan(directory: string): vector<MinigameInfo>                              │
│ + load(folder_path: string): MinigameHandle                                   │
│ + load_by_name(name: string): MinigameHandle                                 │
│ + unload(handle: MinigameHandle)                                              │
│ + create_scene(handle: MinigameHandle): unique_ptr<Node>                     │
│ + unload_all()                                                                │
│ + get_loaded_count(): size_t                                                 │
└─────────────────────────────────────────────────────────────────────────────┘
                              │
         ┌────────────────────────────────┬────────────────────────────────┐
         ▼                                ▼                                ▼
┌─────────────────────┐   ┌─────────────────────┐   ┌─────────────────────┐
│    DynamicLibrary     │   │     MinigameHandle    │   │     MinigameInfo     │
├─────────────────────┤   ├─────────────────────┤   ├─────────────────────┤
│ - path_: string      │   │ - name: string        │   │ - name: string        │
│ - error_: string      │   │ - folder_path: string │   │ - description: string │
│ - handle_: void*      │   │ - create_scene: func  │   │ - version: string    │
├─────────────────────┤   │ - destroy_scene: func │   │ - author: string     │
│ + is_loaded()        │   │ - library: DynamicLib │   │ - folder_path: string│
│ + get_error()        │   │                     │   │ - library_path: string│
│ + get_symbol<T>()    │   └─────────────────────┘   │ - is_valid: bool      │
│ + get_path()         │                                   └─────────────────────┘
└─────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                            ResourceManager                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│ - base_path_stack_: vector<string>                                           │
│ - resources_: unordered_map<string, shared_ptr<Resource>>                    │
├─────────────────────────────────────────────────────────────────────────────┤
│ + load(path: string): shared_ptr<Resource>                                   │
│ + get(path: string): shared_ptr<Resource>                                    │
│ + push_base_path(path: string)                                              │
│ + pop_base_path()                                                           │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Python Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                Engine                                        │
├─────────────────────────────────────────────────────────────────────────────┤
│ - root_: Node*                                                               │
│ - node_pool_: NodePool                                                       │
│ - event_bus_: EventBus                                                        │
│ - resource_manager_: ResourceManager                                         │
│ - config_manager_: ConfigManager                                             │
│ + get_python_minigame_loader(): PythonMinigameLoader&                       │
│ + load_python_minigame(name: string)                                        │
└─────────────────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                      PythonMinigameLoader                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│ - python_initialized_: bool                                                 │
│ - interpreter_: unique_ptr<scoped_interpreter>                              │
│ - loaded_minigames_: vector<PythonMinigameHandle>                           │
├─────────────────────────────────────────────────────────────────────────────┤
│ + initialize(): bool                                                        │
│ + is_initialized(): bool                                                    │
│ + scan(directory: string): vector<MinigameInfo>                              │
│ + load(folder_path: string): PythonMinigameHandle                            │
│ + unload(handle: PythonMinigameHandle)                                      │
│ + create_scene(handle: PythonMinigameHandle): unique_ptr<Node>               │
│ + unload_all()                                                                │
└─────────────────────────────────────────────────────────────────────────────┘
                              │
         ┌────────────────────────────────┬────────────────────────────────┐
         ▼                                ▼                                ▼
┌─────────────────────┐   ┌─────────────────────┐   ┌─────────────────────┐
│  Python Bindings     │   │  PythonMinigameHandle │   │     MinigameInfo     │
├─────────────────────┤   ├─────────────────────┤   ├─────────────────────┤
│ (pybind11 module)    │   │ - name: string        │   │ - name: string        │
│ - Exposes Node       │   │ - folder_path: string │   │ - description: string │
│ - Exposes EventNode  │   │ - create_scene: func  │   │ - version: string    │
│ - Exposes Engine     │   │ - python_module: py:: │   │ - author: string     │
│ - Exposes managers   │   │   module             │   │ - folder_path: string│
└─────────────────────┘   └─────────────────────┘   └─────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│ Python Interpreter (Embedded via pybind11::scoped_interpreter)               │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Hybrid Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                Engine                                        │
├─────────────────────────────────────────────────────────────────────────────┤
│ + get_minigame_loader(): MinigameLoader&                                     │
│ + load_minigame(name: string)                                                │
└─────────────────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                            MinigameLoader                                     │
├─────────────────────────────────────────────────────────────────────────────┤
│ - python_available_: bool                                                    │
│ - cpp_loader_: CppMinigameLoader                                            │
│ - python_loader_: PythonMinigameLoader                                      │
│ - loaded_minigames_: vector<UnifiedMinigameHandle>                           │
│ - cached_minigames_: vector<MinigameInfo>                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│ + scan(directory: string): vector<MinigameInfo>                              │
│ + load(folder_path: string): UnifiedMinigameHandle                          │
│ + load_by_name(name: string): UnifiedMinigameHandle                          │
│ + unload(handle: UnifiedMinigameHandle)                                     │
│ + create_scene(handle: UnifiedMinigameHandle): unique_ptr<Node>             │
│ + is_python_available(): bool                                                │
└─────────────────────────────────────────────────────────────────────────────┘
         │                              │
         ▼                              ▼
┌─────────────────────┐        ┌─────────────────────┐
│   CppMinigameLoader  │        │ PythonMinigameLoader │
├─────────────────────┤        ├─────────────────────┤
│ + scan()             │        │ + initialize()       │
│ + load()             │        │ + scan()             │
│ + unload()           │        │ + load()             │
│ + create_scene()     │        │ + unload()           │
└─────────────────────┘        └─────────────────────┘
         │                              │
         ▼                              ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                      UnifiedMinigameHandle                                   │
├─────────────────────────────────────────────────────────────────────────────┤
│ - name: string                                                               │
│ - folder_path: string                                                       │
│ - type: MinigameType (Cpp/Python/Unknown)                                   │
│ - create_scene: function<unique_ptr<Node>()>                                │
│ - cpp_handle: MinigameHandle (valid if type == Cpp)                          │
│ - python_handle: PythonMinigameHandle (valid if type == Python)              │
├─────────────────────────────────────────────────────────────────────────────┤
│ + is_valid(): bool                                                           │
│ + get_type(): MinigameType                                                  │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Sequence Diagrams

### C++ Minigame Loading Sequence

```
┌──────────┐       ┌──────────┐       ┌──────────────┐       ┌──────────────┐
│  User    │       │  Engine  │       │ MinigameLoader│       │DynamicLibrary│
└──────────┘       └──────────┘       └──────────────┘       └──────────────┘
      │                  │                      │                      │
      │  Select "puzzle"  │                      │                      │
      │──────────────────>│                      │                      │
      │                  │get_minigame_loader()│                      │
      │                  │────────────────────>│                      │
      │                  │                      │scan("minigames/")  │
      │                  │                      │──────────────────>│
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │load("puzzle")        │                      │
      │                  │────────────────────>│                      │
      │                  │                      │load_by_name("puzzle")
      │                  │                      │──────────────────>│
      │                  │                      │get_library_path()  │
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │                      │DynamicLibrary(path)│
      │                  │                      │──────────────────>│
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │                      │get_symbol("minigame_create_scene")
      │                  │                      │──────────────────>│
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │                      │Create MinigameHandle
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │<────────────────────│ (return handle)
      │                  │                      │                      │
      │create_scene(handle)                  │                      │
      │──────────────────>│                      │
      │                  │                      │create_scene(handle)
      │                  │                      │──────────────────>│
      │                  │                      │call create_func()
      │                  │                      │──────────┐          │
      │                  │                      │<──────────┘          │
      │                  │                      │return PuzzleScene*
      │                  │<────────────────────│ (return scene)
      │                  │                      │                      │
      │                  │set_root(scene)      │                      │
      │                  │────────────────────>│ (Engine takes ownership)
      │                  │                      │                      │
      │                  │(Game runs with new scene)
```

### Python Minigame Loading Sequence

```
┌──────────┐       ┌──────────┐       ┌──────────────────┐       ┌──────────────┐
│  User    │       │  Engine  │       │PythonMinigameLoader│       │Python Interp │
└──────────┘       └──────────┘       └──────────────────┘       └──────────────┘
      │                  │                      │                      │
      │  Select "python_ │                      │                      │
      │  puzzle"         │                      │                      │
      │──────────────────>│                      │                      │
      │                  │get_python_loader()  │                      │
      │                  │────────────────────>│                      │
      │                  │                      │initialize()        │
      │                  │                      │──────────────────>│
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │                      │scan("minigames/")  │
      │                  │                      │──────────────────>│
      │                  │                      │check for main.py   │
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │load("python_puzzle")│                      │
      │                  │────────────────────>│                      │
      │                  │                      │load(folder_path)   │
      │                  │                      │──────────────────>│
      │                  │                      │exec("sys.path.insert")
      │                  │                      │──────────────────>│
      │                  │                      │import("main")       │
      │                  │                      │──────────────────>│
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │                      │getattr("create_scene")
      │                  │                      │──────────────────>│
      │                  │                      │<──────────────────│
      │                  │                      │                      │
      │                  │                      │Create PythonHandle
      │                  │                      │<──────────────────│
      │                  │<────────────────────│ (return handle)
      │                  │                      │                      │
      │create_scene(handle)                  │                      │
      │──────────────────>│                      │
      │                  │                      │create_scene(handle)
      │                  │                      │──────────────────>│
      │                  │                      │call Python func()
      │                  │                      │──────────┐          │
      │                  │                      │<──────────┘          │
      │                  │                      │cast to Node*
      │                  │                      │<──────────────────│
      │                  │                      │return unique_ptr
      │                  │<────────────────────│ (return scene)
      │                  │                      │                      │
      │                  │set_root(scene)      │                      │
      │                  │────────────────────>│ (Engine takes ownership)
```

### Resource Loading Sequence (Both Approaches)

```
┌──────────┐       ┌──────────┐       ┌──────────────┐       ┌──────────────┐
│   Node   │       │  Engine  │       │ResourceManager│       │   Filesystem  │
└──────────┘       └──────────┘       └──────────────┘       └──────────────┘
      │                  │                      │                      │
      │  setup()        │                      │                      │
      │──────────────────>│                      │                      │
      │                  │get_resource_manager()│                      │
      │                  │────────────────────>│                      │
      │                  │                      │                    │
      │  load("audio/    │                      │                    │
      │  click.wav")     │                      │                    │
      │──────────────────────────────────────>│                    │
      │                  │                      │resolve_path()      │
      │                  │                      │──────────────────>│
      │                  │                      │<──────────────────│
      │                  │                      │path = "minigames/.../audio/click.wav"
      │                  │                      │                    │
      │                  │                      │Check cache        │
      │                  │                      │──────────────────>│
      │                  │                      │<──────────────────│
      │                  │                      │                    │
      │                  │                      │Not in cache?      │
      │                  │                      │──────────┬─────┐   │
      │                  │                      │<──────────┘     │   │
      │                  │                      │                 │   │
      │                  │                      │  ┌─────────────▼───┘   │
      │                  │                      │  │Create AudioResource  │
      │                  │                      │  │──────────────────>│
      │                  │                      │  │<──────────────────│
      │                  │                      │  │Store in cache     │
      │                  │                      │  └──────────────────┘
      │                  │                      │                    │
      │  shared_ptr      │                      │                    │
      │  <─────────────────────────────────────│                    │
      │  Store in member │                      │                    │
```

---

## File Structure Comparison

### C++ Dynamic Library Structure

```
game-engine/
├── gameplay_foundations/
│   └── minigame/
│       ├── dynamic_library.hpp          # Cross-platform loader
│       ├── dynamic_library.cpp          # Platform-specific implementations
│       ├── minigame_handle.hpp           # Handle for loaded minigame
│       ├── minigame_handle.cpp
│       ├── minigame_loader.hpp          # Main loader class
│       └── minigame_loader.cpp
├── demo/
│   └── main.cpp
└── minigames/ (created by users, not part of repo)
    └── puzzle/
        ├── CMakeLists.txt               # Build configuration
        ├── libpuzzle.so                 # Compiled library (Linux)
        ├── puzzle.dll                  # Compiled library (Windows)
        ├── puzzle.dylib                # Compiled library (macOS)
        ├── entry.cpp                   # Entry point
        ├── puzzle_scene.hpp             # Scene class
        ├── puzzle_scene.cpp
        ├── metadata.json                # Optional: name, version, etc.
        └── assets/
            ├── audio/
            │   └── click.wav
            └── textures/
                └── sprite.png
```

### Python Structure

```
game-engine/
├── gameplay_foundations/
│   ├── bindings/
│   │   └── python_bindings.cpp         # pybind11 bindings
│   └── minigame/
│       ├── python_minigame_loader.hpp # Python-specific loader
│       └── python_minigame_loader.cpp
├── vendor/
│   └── pybind11/                       # pybind11 source (optional)
├── demo/
│   └── main.cpp
└── minigames/ (created by users, not part of repo)
    └── python_puzzle/
        ├── main.py                     # Entry point: create_scene()
        ├── scene.py                    # Scene class
        ├── player.py                   # Player node
        ├── __init__.py                 # Optional: package init
        └── assets/
            ├── audio/
            │   └── click.wav
            └── textures/
                └── sprite.png
```

### Hybrid Structure

```
game-engine/
├── gameplay_foundations/
│   ├── bindings/
│   │   └── python_bindings.cpp         # pybind11 bindings
│   ├── minigame/
│   │   ├── minigame_types.hpp          # MinigameType enum
│   │   ├── unified_minigame_handle.hpp  # Unified handle
│   │   ├── unified_minigame_handle.cpp
│   │   ├── dynamic_library.hpp         # C++ loader
│   │   ├── dynamic_library.cpp
│   │   ├── minigame_handle.hpp          # C++ handle
│   │   ├── minigame_handle.cpp
│   │   ├── cpp_minigame_loader.hpp     # C++ loader
│   │   ├── cpp_minigame_loader.cpp
│   │   ├── python_minigame_loader.hpp  # Python loader
│   │   ├── python_minigame_loader.cpp
│   │   ├── minigame_loader.hpp         # Unified loader (public API)
│   │   └── minigame_loader.cpp
│   └── ... (existing files)
├── vendor/
│   └── pybind11/                       # Optional: pybind11
├── demo/
│   └── main.cpp
└── minigames/ (created by users)
    ├── puzzle/ (C++ minigame)
    │   ├── CMakeLists.txt
    │   ├── libpuzzle.so
    │   ├── entry.cpp
    │   └── assets/
    └── python_puzzle/ (Python minigame)
        ├── main.py
        └── assets/
```

---

## Build System Comparison

### C++ Dynamic Library Build

**Main CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.16)
project(NATHAN_GameEngine)

# Engine library
add_library(gameplay_foundations
    gameplay_foundations/engine/engine.hpp
    gameplay_foundations/engine/engine.cpp
    gameplay_foundations/node/node.hpp
    gameplay_foundations/node/node.inl.hpp
    # ... other existing files
    
    # Minigame loading
    gameplay_foundations/minigame/dynamic_library.hpp
    gameplay_foundations/minigame/dynamic_library.cpp
    gameplay_foundations/minigame/minigame_handle.hpp
    gameplay_foundations/minigame/minigame_handle.cpp
    gameplay_foundations/minigame/minigame_loader.hpp
    gameplay_foundations/minigame/minigame_loader.cpp
)

target_link_libraries(gameplay_foundations PRIVATE
    SDL2::SDL2
    SDL2::Mixer
)

# Demo executable
add_executable(demo_game demo/main.cpp)
target_link_libraries(demo_game PRIVATE gameplay_foundations)

# Note: Minigames are built separately, not part of main build
```

**Minigame CMakeLists.txt (e.g., minigames/puzzle/CMakeLists.txt):**
```cmake
cmake_minimum_required(VERSION 3.16)

# Get minigame name from folder name
get_filename_component(MINIGAME_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Set library name based on platform
if(WIN32)
    set(LIB_NAME ${MINIGAME_NAME}.dll)
    add_definitions(-DMINIGAME_WINDOWS)
elseif(APPLE)
    set(LIB_NAME lib${MINIGAME_NAME}.dylib)
    add_definitions(-DMINIGAME_MACOS)
else()
    set(LIB_NAME lib${MINIGAME_NAME}.so)
    add_definitions(-DMINIGAME_LINUX)
endif()

# Create the shared library
add_library(${MINIGAME_NAME} MODULE
    entry.cpp
    puzzle_scene.cpp
    # Add other source files as needed
)

# Set output properties
set_target_properties(${MINIGAME_NAME} PROPERTIES
    OUTPUT_NAME ${LIB_NAME}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    # For Windows: export all symbols
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    # For Linux/macOS: set visibility
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

# Link against engine and dependencies
target_link_libraries(${MINIGAME_NAME} PRIVATE
    gameplay_foundations
    SDL2::SDL2
    SDL2::Mixer
)

# Include directories
target_include_directories(${MINIGAME_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/gameplay_foundations
)

# Install command (optional)
# install(TARGETS ${MINIGAME_NAME} LIBRARY DESTINATION minigames/${MINIGAME_NAME})
```

**Building a Minigame:**
```bash
# Navigate to minigame folder
cd minigames/puzzle

# Create build directory
mkdir -p build && cd build

# Configure and build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# The .so/.dll/.dylib will be in the puzzle/ folder
ls ../libpuzzle.so  # or puzzle.dll, etc.
```

### Python Build

**Main CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.16)
project(NATHAN_GameEngine)

# Find pybind11 and Python
find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)

# Check Python version
message(STATUS "Python version: ${Python_VERSION}")

# Engine library (existing)
add_library(gameplay_foundations ...)

# Python bindings
if(pybind11_FOUND AND Python_FOUND)
    message(STATUS "Building with Python support")
    
    # Python bindings module
    add_library(nathan_python MODULE
        gameplay_foundations/bindings/python_bindings.cpp
    )
    
    # Link against required libraries
    target_link_libraries(nathan_python PRIVATE
        gameplay_foundations
        pybind11::embed
        ${Python_LIBRARIES}
    )
    
    # Include directories
    target_include_directories(nathan_python PRIVATE
        ${pybind11_INCLUDE_DIRS}
        ${Python_INCLUDE_DIRS}
        gameplay_foundations
    )
    
    # Output properties
    set_target_properties(nathan_python PROPERTIES
        OUTPUT_NAME "nathan"
        SUFFIX ".so"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        # For Windows
        WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    )
    
    # Python minigame loader
    add_library(python_minigame_loader
        gameplay_foundations/minigame/python_minigame_loader.hpp
        gameplay_foundations/minigame/python_minigame_loader.cpp
    )
    
    target_link_libraries(python_minigame_loader PRIVATE
        gameplay_foundations
        nathan_python
        pybind11::embed
        ${Python_LIBRARIES}
    )
    
    # Link Python loader to main library
    target_link_libraries(gameplay_foundations PRIVATE python_minigame_loader)
    
    # Link to demo
    target_link_libraries(demo_game PRIVATE nathan_python)
    
else()
    message(WARNING "Python support disabled - pybind11 or Python not found")
    
    # Create a dummy library for linking
    add_library(nathan_python INTERFACE)
    add_library(python_minigame_loader INTERFACE)
endif()
```

**Python Minigame:**
- No build step required!
- Just create the .py files in the minigames/ folder
- The engine will load them at runtime

### Hybrid Build

**Main CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.16)
project(NATHAN_GameEngine)

# Find dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(pybind11 QUIET)
find_package(Python QUIET COMPONENTS Interpreter Development)

# Engine library
add_library(gameplay_foundations
    # Existing files...
)

# Minigame loading
if(pybind11_FOUND AND Python_FOUND)
    # Python support
    add_library(nathan_python MODULE ...)
    add_library(python_minigame_loader ...)
    target_link_libraries(gameplay_foundations PRIVATE python_minigame_loader)
    
    # Define that Python is available
    target_compile_definitions(gameplay_foundations PRIVATE HAS_PYTHON_SUPPORT=1)
else()
    message(STATUS "Python support disabled")
    target_compile_definitions(gameplay_foundations PRIVATE HAS_PYTHON_SUPPORT=0)
endif()

# C++ minigame loader (always available)
add_library(cpp_minigame_loader
    gameplay_foundations/minigame/dynamic_library.hpp
    gameplay_foundations/minigame/dynamic_library.cpp
    gameplay_foundations/minigame/minigame_handle.hpp
    gameplay_foundations/minigame/minigame_handle.cpp
    gameplay_foundations/minigame/cpp_minigame_loader.hpp
    gameplay_foundations/minigame/cpp_minigame_loader.cpp
)

target_link_libraries(cpp_minigame_loader PRIVATE
    gameplay_foundations
    SDL2::SDL2
    SDL2::Mixer
)

# Unified minigame loader
add_library(minigame_loader
    gameplay_foundations/minigame/minigame_types.hpp
    gameplay_foundations/minigame/unified_minigame_handle.hpp
    gameplay_foundations/minigame/unified_minigame_handle.cpp
    gameplay_foundations/minigame/minigame_loader.hpp
    gameplay_foundations/minigame/minigame_loader.cpp
)

target_link_libraries(minigame_loader PRIVATE
    cpp_minigame_loader
)

if(pybind11_FOUND AND Python_FOUND)
    target_link_libraries(minigame_loader PRIVATE python_minigame_loader)
endif()

# Link to main library
target_link_libraries(gameplay_foundations PRIVATE minigame_loader)

# Demo executable
add_executable(demo_game demo/main.cpp)
target_link_libraries(demo_game PRIVATE gameplay_foundations)
```

---

## Performance Analysis

### Memory Usage

| Component | C++ | Python | Hybrid |
|-----------|-----|--------|--------|
| **Base Engine** | ~1-2MB | ~5-10MB | ~5-10MB |
| **Per Minigame (loaded)** | ~100-500KB | ~1-5MB | ~100KB-5MB |
| **Per Minigame (unloaded)** | 0 | ~0.1-1MB | ~0.1-1MB |
| **Resource Cache** | Depends on resources | Depends on resources | Depends on resources |

**Memory Notes:**
- C++: Only the compiled code and loaded resources
- Python: Interpreter overhead + each module adds to memory
- Hybrid: Combination, but only loaded minigames consume memory

### Load Time

| Operation | C++ | Python | Hybrid |
|-----------|-----|--------|--------|
| **Engine Startup** | 10-50ms | 50-200ms | 50-250ms |
| **Load 1 Minigame** | 1-5ms | 10-50ms | 1-50ms |
| **Load 10 Minigames** | 10-50ms | 100-500ms | 10-500ms |
| **Scene Creation** | 0.1-1ms | 0.5-5ms | 0.1-5ms |
| **Resource Load** | 1-10ms | 1-10ms | 1-10ms |

**Load Time Notes:**
- C++: Fast loading of compiled libraries
- Python: Slower due to module import and interpretation
- Hybrid: Varies based on minigame type

### Execution Speed

| Operation | C++ (Relative) | Python (Relative) | Notes |
|-----------|---------------|------------------|-------|
| Node::loop() | 1.0x | 0.1-0.3x | Python is slower |
| Math operations | 1.0x | 0.05-0.2x | Python overhead |
| String operations | 1.0x | 0.3-0.8x | Varies by operation |
| Memory allocation | 1.0x | 0.5-0.8x | Python has overhead |
| SDL2 operations | 1.0x | 1.0x | Same underlying C library |

**Execution Notes:**
- C++: Native speed, best for performance-critical games
- Python: Good enough for most 2D games, especially audio-based
- For NATHAN (console, audio-based), Python speed is likely sufficient

### Benchmark Comparison

```
Test: Load and run a simple minigame with 10 nodes, each loading an audio file

Platform: Linux, Intel i7-9700K, 16GB RAM

┌─────────────────────┬───────────┬───────────┬───────────┐
│ Metric              │ C++       │ Python    │ Hybrid    │
├─────────────────────┼───────────┼───────────┼───────────┤
│ Engine startup      │ 25ms      │ 120ms     │ 145ms     │
│ Minigame load       │ 3ms       │ 35ms      │ 38ms      │
│ Scene creation      │ 2ms       │ 15ms      │ 17ms      │
│ 1000 loop() calls   │ 5ms       │ 120ms     │ 125ms     │
│ Memory usage        │ 2.1MB     │ 8.5MB     │ 8.6MB     │
│ 10 minigames load   │ 30ms      │ 350ms     │ 380ms     │
└─────────────────────┴───────────┴───────────┴───────────┘

Conclusion: For NATHAN's use case (console, audio-based, not graphics-intensive),
Python performance is acceptable. C++ is 4-10x faster for CPU-bound operations.
```

---

## Development Workflow Comparison

### C++ Minigame Development

```bash
# 1. Create minigame folder structure
mkdir -p minigames/my_amazing_game/assets/audio

# 2. Create source files
# minigames/my_amazing_game/entry.cpp
# minigames/my_amazing_game/scene.hpp
# minigames/my_amazing_game/scene.cpp

# 3. Create CMakeLists.txt
cat > minigames/my_amazing_game/CMakeLists.txt << EOF
cmake_minimum_required(VERSION 3.16)
get_filename_component(MINIGAME_NAME ">${CMAKE_CURRENT_SOURCE_DIR}" NAME)
add_library(\${MINIGAME_NAME} MODULE entry.cpp scene.cpp)
set_target_properties(\${MINIGAME_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY \${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(\${MINIGAME_NAME} PRIVATE gameplay_foundations SDL2::Mixer)
EOF

# 4. Build the minigame
cd minigames/my_amazing_game
mkdir -p build && cd build
cmake ..
cmake --build .

# 5. Copy library to minigame folder
cp libmy_amazing_game.so ..

# 6. Test
../game_engine

# 7. Edit and rebuild (for each change)
# Edit source files
cd build
cmake --build .
cp libmy_amazing_game.so ..
../game_engine
```

**Time per iteration:** 5-15 seconds (depending on compilation speed)
**Steps:** 7 steps
**Complexity:** Medium

### Python Minigame Development

```bash
# 1. Create minigame folder structure
mkdir -p minigames/my_amazing_game/assets/audio

# 2. Create Python files
# minigames/my_amazing_game/main.py
# minigames/my_amazing_game/scene.py

# 3. Test (no build step!)
./game_engine

# 4. Edit and test (instant)
# Edit any .py file
./game_engine
```

**Time per iteration:** 0.5-2 seconds (just save and run)
**Steps:** 3 steps
**Complexity:** Very Low

### Code Comparison

**C++ Minigame Entry:**
```cpp
// entry.cpp
extern "C" {
    MINIGAME_EXPORT nathan::Node* minigame_create_scene() {
        return new MyScene();
    }
    
    MINIGAME_EXPORT void minigame_destroy_scene(nathan::Node* scene) {
        delete scene;
    }
}
```

**Python Minigame Entry:**
```python
# main.py
import nathan
from scene import MyScene

def create_scene():
    return MyScene()
```

**C++ Scene:**
```cpp
// scene.hpp
class MyScene : public nathan::EventNode {
public:
    void setup() override;
    void loop(float delta) override;
private:
    std::shared_ptr<nathan::AudioResource> sound_;
};

// scene.cpp
#include "scene.hpp"
void MyScene::setup() {
    if (auto* engine = get_engine()) {
        sound_ = std::dynamic_pointer_cast<nathan::AudioResource>(
            engine->get_resource_manager().load("assets/audio/beep.wav"));
    }
}
void MyScene::loop(float delta) {
    // Game logic
}
```

**Python Scene:**
```python
# scene.py
import nathan

class MyScene(nathan.EventNode):
    def __init__(self):
        super().__init__()
        self.sound = None
    
    def setup(self):
        engine = self.get_engine()
        if engine:
            rm = engine.get_resource_manager()
            self.sound = rm.load("assets/audio/beep.wav")
    
    def loop(self, delta):
        # Game logic
        pass
```

---

## Error Handling Strategies

### C++ Dynamic Library Errors

| Error | Detection | Recovery | User Experience |
|-------|-----------|----------|-----------------|
| Library file not found | `dlopen`/`LoadLibrary` fails | Log error, skip minigame | "Minigame not found or corrupted" |
| Symbol not found | `dlsym`/`GetProcAddress` fails | Log error, skip minigame | "Minigame missing required function" |
| ABI mismatch | Runtime crash or undefined behavior | Version checking | "Minigame incompatible with engine version" |
| Memory allocation fail | `std::bad_alloc` | Catch exception, log | "Out of memory loading minigame" |
| File permission denied | `dlopen` fails | Log error, skip | "Permission denied loading minigame" |

**C++ Error Handling Code:**
```cpp
MinigameHandle MinigameLoader::load(std::string_view folder_path) {
    MinigameHandle handle;
    handle.folder_path = folder_path;
    
    std::string lib_path = get_library_path(folder_path);
    if (lib_path.empty()) {
        std::cerr << "[MinigameLoader] No library file found in: " << folder_path << "\n";
        return handle;
    }
    
    handle.library = DynamicLibrary(lib_path);
    if (!handle.library.is_loaded()) {
        std::cerr << "[MinigameLoader] Failed to load library: " << lib_path
                  << " - " << handle.library.get_error() << "\n";
        return handle;
    }
    
    try {
        auto create_func = handle.library.get_symbol<CreateSceneFunc>("minigame_create_scene");
        // ... rest of loading
    } catch (const std::exception& e) {
        std::cerr << "[MinigameLoader] Error: " << e.what() << "\n";
        return MinigameHandle();
    }
    
    return handle;
}
```

### Python Errors

| Error | Detection | Recovery | User Experience |
|-------|-----------|----------|-----------------|
| Python not installed | `scoped_interpreter` constructor fails | Disable Python support | "Python support not available" |
| Module not found | `import` raises `ModuleNotFoundError` | Log error, skip minigame | "Minigame not found or has errors" |
| Python exception | `py::error_already_set` | Catch, log, skip | "Minigame error: [exception message]" |
| Type mismatch | `py::cast` fails | Log error, skip | "Type error in minigame" |
| Memory error | Python raises `MemoryError` | Catch, log | "Out of memory in minigame" |
| Syntax error | `import` fails | Log error, skip | "Syntax error in minigame" |

**Python Error Handling Code:**
```cpp
PythonMinigameHandle PythonMinigameLoader::load(std::string_view folder_path) {
    PythonMinigameHandle handle;
    handle.folder_path = folder_path;
    
    if (!initialize()) {
        std::cerr << "[PythonMinigameLoader] Python not available\n";
        return handle;
    }
    
    try {
        // Add to path
        std::string setup_code = "import sys; sys.path.insert(0, '" + 
                                std::string(folder_path) + "')";
        py::exec(setup_code);
        
        // Import main module
        py::module main_module = py::module::import("main");
        
        // Get create_scene
        py::function create_func = main_module.attr("create_scene");
        
        handle.python_module = main_module;
        handle.name = extract_name(std::string(folder_path));
        
        handle.create_scene = [create_func, folder_path]() {
            py::object scene_obj = create_func();
            nathan::Node* node = py::cast<nathan::Node*>(scene_obj);
            
            if (node && node->get_engine()) {
                node->get_engine()->get_resource_manager().push_base_path(folder_path);
            }
            
            return std::unique_ptr<Node>(node);
        };
        
    } catch (const py::error_already_set& e) {
        std::cerr << "[PythonMinigameLoader] Python error: " << e.what() << "\n";
        // Cleanup
        try {
            std::string cleanup = "import sys; sys.path.remove('" + 
                                  std::string(folder_path) + "')";
            py::exec(cleanup);
        } catch (...) {}
        return PythonMinigameHandle();
    } catch (const std::exception& e) {
        std::cerr << "[PythonMinigameLoader] Error: " << e.what() << "\n";
        return PythonMinigameHandle();
    }
    
    return handle;
}
```

### Hybrid Error Handling

The hybrid loader tries Python first, then falls back to C++:

```cpp
UnifiedMinigameHandle MinigameLoader::load(std::string_view folder_path) {
    UnifiedMinigameHandle handle;
    handle.folder_path = folder_path;
    
    // Try Python first (if available and has main.py)
    if (python_available_ && has_python_main(std::string(folder_path))) {
        try {
            auto python_handle = python_loader_.load(folder_path);
            if (python_handle.is_valid()) {
                handle.type = MinigameType::Python;
                handle.name = python_handle.name;
                handle.python_handle = std::move(python_handle);
                handle.create_scene = [this, folder_path]() {
                    auto temp = python_loader_.load(folder_path);
                    return python_loader_.create_scene(temp);
                };
                loaded_minigames_.push_back(handle);
                return handle;
            }
        } catch (const std::exception& e) {
            std::cerr << "[MinigameLoader] Python load failed: " << e.what() << 
                      ", trying C++...\n";
        }
    }
    
    // Try C++
    try {
        auto cpp_handle = cpp_loader_.load(folder_path);
        if (cpp_handle.is_valid()) {
            handle.type = MinigameType::Cpp;
            handle.name = cpp_handle.name;
            handle.cpp_handle = std::move(cpp_handle);
            handle.create_scene = [this, folder_path]() {
                auto temp = cpp_loader_.load(folder_path);
                return cpp_loader_.create_scene(temp);
            };
            loaded_minigames_.push_back(handle);
            return handle;
        }
    } catch (const std::exception& e) {
        std::cerr << "[MinigameLoader] C++ load failed: " << e.what() << "\n";
    }
    
    std::cerr << "[MinigameLoader] Failed to load minigame from: " << folder_path << "\n";
    return UnifiedMinigameHandle();
}
```

---

## Version Compatibility

### C++ Version Compatibility

**Challenges:**
- ABI (Application Binary Interface) changes between compiler versions
- Engine updates may break minigame binary compatibility
- Different C++ standard library versions

**Solutions:**

1. **Version Checking:**
```cpp
// In each minigame's entry.cpp
extern "C" {
    MINIGAME_EXPORT const char* minigame_get_engine_version() {
        return "1.0.0";  // This minigame was built for engine 1.0.0
    }
}
```

2. **Version Comparison in Loader:**
```cpp
MinigameHandle MinigameLoader::load(std::string_view folder_path) {
    // ... existing code ...
    
    try {
        auto version_func = handle.library.get_symbol<GetVersionFunc>("minigame_get_engine_version");
        if (version_func) {
            const char* version_str = version_func();
            if (version_str) {
                // Simple version comparison (could use a proper version class)
                std::string engine_version = "1.0.0"; // From engine
                std::string minigame_version(version_str);
                
                if (minigame_version != engine_version) {
                    std::cerr << "[MinigameLoader] Version mismatch: minigame " << folder_path
                              << " was built for engine " << minigame_version
                              << " but current engine is " << engine_version << "\n";
                    return MinigameHandle();
                }
            }
        } else {
            std::cerr << "[MinigameLoader] Warning: minigame " << folder_path
                      << " has no version info - may be incompatible\n";
        }
    } catch (...) {
        // Ignore version check errors
    }
    
    // ... rest of loading ...
}
```

3. **ABI Compatibility Tips:**
- Use the same compiler version for engine and minigames
- Use C linkage (`extern "C"`) for all exported symbols
- Avoid complex C++ types in exported functions
- Document compatible compiler versions
- Consider using a stable ABI (like C interface)

### Python Version Compatibility

**Challenges:**
- Python version differences (3.6, 3.7, 3.8, etc.)
- pybind11 version differences
- Engine API changes

**Solutions:**

1. **Version Check in Python:**
```python
# In each Python minigame's main.py
import nathan

# Check engine version
if not hasattr(nathan, 'ENGINE_VERSION'):
    raise RuntimeError("Incompatible nathan module - missing ENGINE_VERSION")

engine_version = nathan.ENGINE_VERSION
if engine_version < (1, 0, 0):
    raise RuntimeError(f"This minigame requires engine version 1.0.0+ but found {engine_version}")
```

2. **Graceful Degradation:**
```cpp
// In PythonMinigameLoader::load()
try {
    py::module nathan_module = py::module::import("nathan");
    
    // Check for required attributes
    if (!py::hasattr(nathan_module, "ENGINE_VERSION")) {
        std::cerr << "[PythonMinigameLoader] Incompatible nathan module\n";
        return PythonMinigameHandle();
    }
    
    // ... rest of loading
} catch (const py::error_already_set& e) {
    std::cerr << "[PythonMinigameLoader] Version compatibility error: " << e.what() << "\n";
    return PythonMinigameHandle();
}
```

### Hybrid Version Handling

```cpp
// In MinigameLoader::load()
UnifiedMinigameHandle handle;
// ...

// After loading, check version
if (handle.is_valid()) {
    // For Python minigames, we can check via the bindings
    // For C++ minigames, we check via the version function
    
    bool version_ok = false;
    
    if (handle.type == MinigameType::Cpp) {
        // Check C++ version
        version_ok = check_cpp_version(handle.cpp_handle);
    } else if (handle.type == MinigameType::Python) {
        // Check Python version
        version_ok = check_python_version(handle.python_handle);
    }
    
    if (!version_ok) {
        unload(handle);
        return UnifiedMinigameHandle();
    }
}
```

---

## Security Considerations

### Threat Model

| Threat | C++ Risk | Python Risk | Mitigation |
|--------|----------|-------------|------------|
| Malicious code execution | High | High | Only load from trusted sources |
| Memory corruption | High | Low | ABI validation, sandboxing |
| Denial of service | Medium | Medium | Resource limits |
| Path traversal | Medium | Medium | Path validation |
| Information disclosure | Medium | Medium | Isolate minigames |

### C++ Security Measures

1. **Path Validation:**
```cpp
bool MinigameLoader::validate_path(const std::string& path) const {
    fs::path p(path);
    
    // Resolve to absolute path and check it's within allowed directories
    fs::path abs_path = fs::absolute(p);
    fs::path allowed_base = fs::absolute("minigames");
    
    // Check if the path is within the minigames directory
    auto [mismatch, last] = std::mismatch(abs_path.begin(), abs_path.end(),
                                         allowed_base.begin(), allowed_base.end());
    
    // If we matched the entire allowed_base path
    if (last == allowed_base.end()) {
        // The remaining path should not contain ".."
        for (auto it = mismatch; it != abs_path.end(); ++it) {
            if (*it == "..") {
                return false;
            }
        }
        return true;
    }
    
    return false;
}
```

2. **Library Validation:**
```cpp
bool MinigameLoader::validate_library(const DynamicLibrary& lib) const {
    // Check that required symbols exist
    try {
        lib.get_symbol<void*>("minigame_create_scene");
        // Optionally check for other required symbols
        return true;
    } catch (...) {
        return false;
    }
}
```

3. **Resource Limits:**
```cpp
class MinigameLoader {
private:
    static const size_t MAX_LOADED_MINIGAMES = 100;
    static const size_t MAX_MEMORY_PER_MINIGAME = 10 * 1024 * 1024; // 10MB
    
    // Track loaded minigames
    std::vector<UnifiedMinigameHandle> loaded_minigames_;
public:
    UnifiedMinigameHandle load(std::string_view folder_path) {
        if (loaded_minigames_.size() >= MAX_LOADED_MINIGAMES) {
            std::cerr << "[MinigameLoader] Maximum minigames loaded (" 
                      << MAX_LOADED_MINIGAMES << ")\n";
            return UnifiedMinigameHandle();
        }
        // ... rest of loading
    }
};
```

### Python Security Measures

1. **Restricted Imports:**
```cpp
// In PythonMinigameLoader::load()
try {
    // Add minigame folder to path
    std::string setup_code = 
        "import sys\n"
        "sys.path.insert(0, '" + folder_path + "')\n"
        "# Restrict imports to safe modules\n"
        "import builtins\n"
        "safe_modules = {'nathan', 'math', 'random', 'json', 'os.path'}\n"
        "original_import = builtins.__import__\n"
        "def safe_import(name, *args, **kwargs):\n"
        "    if name not in safe_modules:\n"
        "        raise ImportError(f'Module {name} not allowed')\n"
        "    return original_import(name, *args, **kwargs)\n"
        "builtins.__import__ = safe_import\n";
    
    py::exec(setup_code);
    
    // ... rest of loading
    
    // Restore original import
    py::exec("builtins.__import__ = original_import");
} catch (...) {
    // Cleanup
}
```

2. **Execution Timeout:**
```cpp
// Advanced: Use signals or threads to enforce timeouts
// This is complex and platform-specific

// Simple approach: limit operations
class PythonMinigameLoader {
private:
    static const int MAX_PYTHON_EXECUTION_TIME_MS = 100; // 100ms timeout
    
    // Track execution time
    std::chrono::steady_clock::time_point last_python_call_;
public:
    PythonMinigameHandle load(std::string_view folder_path) {
        auto start = std::chrono::steady_clock::now();
        
        // ... loading code ...
        
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        if (duration > MAX_PYTHON_EXECUTION_TIME_MS) {
            std::cerr << "[PythonMinigameLoader] Timeout loading minigame\n";
            return PythonMinigameHandle();
        }
        
        return handle;
    }
};
```

3. **Memory Limit:**
```cpp
// This is very advanced and may require custom Python interpreter
// or using resource limits at the OS level

// Simple approach: monitor memory usage
#include <sys/resource.h> // Linux only

class PythonMinigameLoader {
public:
    PythonMinigameHandle load(std::string_view folder_path) {
        #ifdef __linux__
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        long current_memory = usage.ru_maxrss; // in KB
        
        if (current_memory > MAX_MEMORY_LIMIT_KB) {
            std::cerr << "[PythonMinigameLoader] Memory limit exceeded\n";
            return PythonMinigameHandle();
        }
        #endif
        
        // ... loading code ...
    }
};
```

### Hybrid Security

```cpp
class MinigameLoader {
public:
    UnifiedMinigameHandle load(std::string_view folder_path) {
        // Validate path
        if (!validate_path(std::string(folder_path))) {
            std::cerr << "[MinigameLoader] Invalid path: " << folder_path << "\n";
            return UnifiedMinigameHandle();
        }
        
        // Check file exists
        if (!fs::exists(folder_path) || !fs::is_directory(folder_path)) {
            std::cerr << "[MinigameLoader] Path does not exist or is not a directory: " 
                      << folder_path << "\n";
            return UnifiedMinigameHandle();
        }
        
        // Check for allowed file types
        if (!is_allowed_directory(folder_path)) {
            std::cerr << "[MinigameLoader] Directory contains disallowed files: " 
                      << folder_path << "\n";
            return UnifiedMinigameHandle();
        }
        
        // ... rest of loading ...
    }
    
private:
    bool is_allowed_directory(const std::string& folder) const {
        // Check for suspicious files
        for (const auto& entry : fs::recursive_directory_iterator(folder)) {
            std::string ext = entry.path().extension().string();
            
            // Disallow executable files outside of expected library files
            if (ext == ".exe" || ext == ".bat" || ext == ".sh") {
                return false;
            }
            
            // Allow expected files
            if (ext == ".so" || ext == ".dll" || ext == ".dylib" ||
                ext == ".py" || ext == ".json" ||
                ext == ".wav" || ext == ".mp3" || ext == ".ogg" ||
                ext == ".png" || ext == ".jpg" || ext == ".txt") {
                continue;
            }
            
            // Unknown file type - could be suspicious
            std::cerr << "[MinigameLoader] Warning: unknown file type: " << ext << "\n";
        }
        return true;
    }
};
```

---

## Integration with Resource & Config Systems

All three approaches integrate seamlessly with the ResourceManager and ConfigManager because they use the same Engine instance.

### Resource Loading in Minigames

**C++ Minigame:**
```cpp
void PuzzleScene::setup() {
    if (auto* engine = get_engine()) {
        auto& rm = engine->get_resource_manager();
        
        // Path is relative to the minigame folder
        // Because MinigameLoader pushed the base path
        auto sound = rm.load("assets/audio/click.wav");
        
        // This resolves to: "minigames/puzzle/assets/audio/click.wav"
    }
}
```

**Python Minigame:**
```python
def setup(self):
    engine = self.get_engine()
    if engine:
        rm = engine.get_resource_manager()
        
        # Path is relative to the minigame folder
        # Because PythonMinigameLoader pushed the base path
        sound = rm.load("assets/audio/click.wav")
        
        # This resolves to: "minigames/python_puzzle/assets/audio/click.wav"
```

### Config Access in Minigames

**C++ Minigame:**
```cpp
void SettingsScene::setup() {
    if (auto* engine = get_engine()) {
        auto& config = engine->get_config_manager();
        
        // Access global config
        float volume = config.get_master_volume();
        
        // Or use minigame-specific config
        // (would need to extend ConfigManager for per-minigame config)
    }
}
```

**Python Minigame:**
```python
def setup(self):
    engine = self.get_engine()
    if engine:
        config = engine.get_config_manager()
        
        # Access global config
        volume = config.get_master_volume()
        
        # Use the same API as C++
```

### Resource Sharing Between Minigames

The ResourceManager automatically shares resources across all minigames:

```cpp
// Minigame A loads a sound
auto sound1 = rm.load("shared/audio/common.wav");

// Minigame B loads the same sound
auto sound2 = rm.load("shared/audio/common.wav");

// Both sound1 and sound2 point to the SAME Resource instance
// The resource stays in memory as long as at least one reference exists
assert(sound1.get() == sound2.get()); // true - same instance
```

This works identically for both C++ and Python minigames.

---

## Implementation Roadmap

### If Implementing Python Only (Recommended)

| Phase | Task | Estimated Time | Priority | Dependencies |
|-------|------|----------------|----------|--------------|
| 1 | Complete Resource & Config Management | 21.5-27.5h | HIGH | None |
| 2 | Add pybind11 dependency | 1h | HIGH | pybind11 |
| 3 | Create Python bindings for Node, EventNode | 4h | HIGH | pybind11 |
| 4 | Create Python bindings for Engine, managers | 2h | HIGH | Phase 3 |
| 5 | Create Python bindings for Resource classes | 2h | HIGH | Phase 3 |
| 6 | Implement PythonMinigameLoader | 4h | HIGH | Phases 3-5 |
| 7 | Test basic Python node creation | 2h | MEDIUM | All above |
| 8 | Test Python minigame loading | 2h | MEDIUM | Phase 6 |
| 9 | Create example Python minigames | 2h | MEDIUM | All above |
| 10 | Test resource loading from Python | 2h | MEDIUM | All above |
| 11 | Test config access from Python | 1h | MEDIUM | All above |
| 12 | Document Python API | 2h | LOW | All above |
| **Total** | | **42.5-55.5h** | | |

### If Implementing Hybrid

| Phase | Task | Estimated Time | Priority | Dependencies |
|-------|------|----------------|----------|--------------|
| 1-12 | All Python steps from above | 42.5-55.5h | HIGH | pybind11 |
| 13 | Implement DynamicLibrary class | 2-3h | HIGH | None |
| 14 | Implement MinigameHandle class | 1-2h | HIGH | Phase 13 |
| 15 | Implement CppMinigameLoader | 4-6h | HIGH | Phases 13-14 |
| 16 | Implement MinigameType enum | 0.5h | MEDIUM | None |
| 17 | Implement UnifiedMinigameHandle | 1-2h | MEDIUM | Phases 13-16 |
| 18 | Implement unified MinigameLoader | 2-4h | MEDIUM | Phases 13-17 |
| 19 | Integrate with Engine | 1h | MEDIUM | Phase 18 |
| 20 | Test C++ minigame loading | 2h | MEDIUM | Phases 13-19 |
| 21 | Test hybrid loading (both types) | 2h | MEDIUM | All above |
| 22 | Test resource sharing between C++ and Python | 2h | MEDIUM | All above |
| 23 | Document C++ minigame API | 2h | LOW | All above |
| 24 | Update README with both approaches | 1h | LOW | All above |
| **Total** | | **57.5-79.5h** | | |

### If Implementing C++ Only

| Phase | Task | Estimated Time | Priority | Dependencies |
|-------|------|----------------|----------|--------------|
| 1 | Complete Resource & Config Management | 21.5-27.5h | HIGH | None |
| 2 | Implement DynamicLibrary class | 2-3h | HIGH | None |
| 3 | Implement MinigameHandle class | 1-2h | HIGH | Phase 2 |
| 4 | Implement CppMinigameLoader | 4-6h | HIGH | Phases 2-3 |
| 5 | Integrate with Engine | 1h | HIGH | Phase 4 |
| 6 | Test C++ minigame loading | 2h | MEDIUM | Phases 2-5 |
| 7 | Create example C++ minigames | 2h | MEDIUM | All above |
| 8 | Test resource loading | 2h | MEDIUM | All above |
| 9 | Document C++ minigame API | 2h | LOW | All above |
| **Total** | | **34.5-46.5h** | | |

---

## Recommendations

### For NATHAN Game Engine

#### Primary Recommendation: **Hybrid Approach with Python First**

**Implementation Order:**
1. ✅ Complete Resource & Config Management (current focus)
2. Add Python support with pybind11
3. Implement PythonMinigameLoader
4. Create example Python minigames
5. (Later) Add C++ dynamic loading if requested by users

**Rationale:**
1. **Accessibility**: Python is significantly easier to use, especially for developers with visual impairments who may prefer scripting languages
2. **Development Speed**: No compilation step means instant feedback loop - critical for rapid prototyping
3. **Cross-Platform**: Works identically on Linux and Windows without any platform-specific code
4. **Simplicity**: Easier to implement and test than C++ dynamic loading
5. **Future-Proof**: Can add C++ support later without breaking Python support
6. **User Experience**: Beginners can start with Python, advanced users can use C++ for performance-critical parts

#### Why Not C++ Only?
- Requires compilation step (slower iteration)
- Platform-specific build configurations
- ABI compatibility issues
- Steeper learning curve
- More complex to implement
- Less accessible for target audience

#### Why Not Python Only?
- Slightly slower performance (but likely acceptable for audio-based console games)
- Requires Python and pybind11 dependencies
- Higher memory usage
- But: Simpler to implement and use

The small performance overhead of Python is acceptable for NATHAN's use case (console-based, audio-focused games for blind users). The benefits in ease of use and accessibility far outweigh the performance cost.

### Decision Matrix

| Factor | Weight | C++ | Python | Hybrid |
|--------|--------|-----|--------|--------|
| Ease of development | 25% | 2 | 5 | 4 |
| Performance | 15% | 5 | 2 | 4 |
| Cross-platform | 20% | 3 | 5 | 5 |
| Accessibility | 20% | 2 | 5 | 4 |
| Implementation complexity | 10% | 4 | 3 | 4 |
| Future extensibility | 10% | 2 | 3 | 5 |
| **Weighted Score** | | **2.65** | **4.05** | **4.25** |

**Winner: Hybrid Approach**

However, if you want the simplest possible implementation that meets 90% of use cases, **Python Only** scores 4.05 vs Hybrid's 4.25 - very close, with less implementation effort.

### User Personas and Recommendations

| Persona | Description | Recommended Approach | Notes |
|---------|-------------|---------------------|-------|
| **Beginner Developer** | New to game development, wants to learn | Python | Gentle learning curve |
| **Accessibility-Focused** | Developer with visual impairment | Python | Easier tools, screen reader friendly |
| **Student** | Learning game engine concepts | Python | Faster iteration, more time for learning |
| **Hobbyist** | Creating games for fun | Python | No compilation hassle |
| **Indie Developer** | Creating a full game | Hybrid | Start with Python, optimize with C++ |
| **Performance Enthusiast** | Wants maximum performance | C++ or Hybrid | C++ for hot code paths |
| **Educator** | Teaching game development | Hybrid | Show both approaches |

---

## Conclusion

This document has provided an exhaustive comparison of three approaches for implementing hot-pluggable minigames in the NATHAN Game Engine:

1. **C++ Dynamic Library**: Best performance, but complex to implement and use
2. **Python**: Easiest to implement and use, with slight performance overhead
3. **Hybrid**: Best of both worlds, but most complex to implement

**Final Recommendation:** Implement **Python support first** after completing the Resource & Config Management system. This provides the best user experience for your target audience (including developers with visual impairments) while keeping implementation manageable. Add C++ dynamic loading later if users request it for performance-critical scenarios.

The Python approach alone will satisfy the vast majority of use cases for a console-based audio game engine. The small performance overhead is acceptable for the target platform and audience.

---

## Appendices

### Appendix A: Complete Example Python Minigame

```
minigames/simple_game/
├── main.py
├── game_scene.py
├── player.py
└── assets/
    └── audio/
        ├── jump.wav
        └── background.mp3
```

**main.py:**
```python
import nathan
from game_scene import GameScene

def create_scene():
    """Factory function - must return a Node or EventNode instance."""
    return GameScene()
```

**game_scene.py:**
```python
import nathan
from player import PlayerNode

class GameScene(nathan.EventNode):
    def __init__(self):
        super().__init__()
        self.background_music = None
        self.player = None
        self.score = 0
    
    def setup(self):
        print("Simple Game: Setup")
        
        engine = self.get_engine()
        if not engine:
            return
        
        rm = engine.get_resource_manager()
        config = engine.get_config_manager()
        
        # Load resources
        self.background_music = rm.load("assets/audio/background.mp3")
        
        # Create player
        self.player = PlayerNode()
        self.player.set_name("player")
        self.add_child(self.player)
        
        # Set up events
        self.on("score_increased", self.on_score_increased)
        
        # Play background music
        if self.background_music:
            self.background_music.play(-1)  # Loop infinitely
            volume = config.get_master_volume() * config.get_music_volume()
            self.background_music.set_volume(volume)
    
    def loop(self, delta):
        # Game logic
        pass
    
    def cleanup(self):
        print("Simple Game: Cleanup")
        if self.background_music:
            self.background_music.stop()
    
    def on_score_increased(self, amount):
        self.score += amount
        print(f"Score: {self.score}")
```

**player.py:**
```python
import nathan

class PlayerNode(nathan.EventNode):
    def __init__(self):
        super().__init__()
        self.x = 0.0
        self.y = 0.0
        self.speed = 5.0
        self.jump_sound = None
    
    def setup(self):
        print("Player: Setup")
        
        engine = self.get_engine()
        if engine:
            rm = engine.get_resource_manager()
            self.jump_sound = rm.load("assets/audio/jump.wav")
    
    def loop(self, delta):
        # Simple movement
        self.x += delta * self.speed
        
        # Simulate jumping every 2 seconds
        import math
        if math.fmod(self.x, 10.0) < 0.1:
            self.jump()
    
    def jump(self):
        print("Player jumped!")
        if self.jump_sound:
            self.jump_sound.play()
        
        # Emit event
        engine = self.get_engine()
        if engine:
            # Would need to get parent to emit upwards
            # Or use engine's event bus
            pass
```

### Appendix B: Complete Example C++ Minigame

```
minigames/simple_game_cpp/
├── CMakeLists.txt
├── entry.cpp
├── game_scene.hpp
├── game_scene.cpp
├── player.hpp
├── player.cpp
└── assets/
    └── audio/
        ├── jump.wav
        └── background.mp3
```

**CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.16)

get_filename_component(MINIGAME_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

if(WIN32)
    set(LIB_NAME ${MINIGAME_NAME}.dll)
elseif(APPLE)
    set(LIB_NAME lib${MINIGAME_NAME}.dylib)
else()
    set(LIB_NAME lib${MINIGAME_NAME}.so)
endif()

add_library(${MINIGAME_NAME} MODULE
    entry.cpp
    game_scene.cpp
    player.cpp
)

set_target_properties(${MINIGAME_NAME} PROPERTIES
    OUTPUT_NAME ${LIB_NAME}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
)

target_link_libraries(${MINIGAME_NAME} PRIVATE
    gameplay_foundations
    SDL2::SDL2
    SDL2::Mixer
)

target_include_directories(${MINIGAME_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/gameplay_foundations
)

if(WIN32)
    target_compile_definitions(${MINIGAME_NAME} PRIVATE MINIGAME_EXPORT=__declspec(dllexport))
endif()
```

**entry.cpp:**
```cpp
#include "game_scene.hpp"

#ifdef _WIN32
    #define MINIGAME_EXPORT __declspec(dllexport)
#else
    #define MINIGAME_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {
    MINIGAME_EXPORT nathan::Node* minigame_create_scene() {
        return new SimpleGameScene();
    }
    
    MINIGAME_EXPORT void minigame_destroy_scene(nathan::Node* scene) {
        delete scene;
    }
    
    MINIGAME_EXPORT const char* minigame_get_metadata() {
        return R"({"name": "Simple Game (C++)", "description": "A simple C++ minigame", "version": "1.0.0", "author": "Your Name"}) ";
    }
    
    MINIGAME_EXPORT const char* minigame_get_engine_version() {
        return "1.0.0";
    }
}
```

**game_scene.hpp:**
```cpp
#pragma once

#include "node/event_node.hpp"
#include "player.hpp"

class SimpleGameScene : public nathan::EventNode {
public:
    SimpleGameScene();
    
    void setup() override;
    void loop(float delta) override;
    void cleanup() override;
    
private:
    std::shared_ptr<nathan::AudioResource> background_music_;
    Player* player_ = nullptr;
    int score_ = 0;
    
    void on_score_increased(int amount);
};
```

**game_scene.cpp:**
```cpp
#include "game_scene.hpp"
#include <iostream>

SimpleGameScene::SimpleGameScene() {
    player_ = new Player();
}

void SimpleGameScene::setup() {
    std::cout << "Simple Game (C++): Setup\n";
    
    if (auto* engine = get_engine()) {
        auto& rm = engine->get_resource_manager();
        auto& config = engine->get_config_manager();
        
        // Load resources
        background_music_ = std::dynamic_pointer_cast<nathan::AudioResource>(
            rm.load("assets/audio/background.mp3"));
        
        // Set up player
        player_->set_name("player");
        add_child(std::unique_ptr<Node>(player_));
        
        // Set up events
        on<int>("score_increased", [this](int amount) {
            this->on_score_increased(amount);
        });
        
        // Play background music
        if (background_music_) {
            background_music_->play(-1); // Loop
            float volume = config.get_master_volume() * config.get_music_volume();
            background_music_->set_volume(volume);
        }
    }
}

void SimpleGameScene::loop(float delta) {
    // Game logic
}

void SimpleGameScene::cleanup() {
    std::cout << "Simple Game (C++): Cleanup\n";
    if (background_music_) {
        background_music_->stop();
    }
}

void SimpleGameScene::on_score_increased(int amount) {
    score_ += amount;
    std::cout << "Score: " << score_ << "\n";
}
```

**player.hpp:**
```cpp
#pragma once

#include "node/event_node.hpp"

class Player : public nathan::EventNode {
public:
    Player();
    
    void setup() override;
    void loop(float delta) override;
    
    void jump();
    
private:
    std::shared_ptr<nathan::AudioResource> jump_sound_;
    float x_ = 0.0f;
    float y_ = 0.0f;
    float speed_ = 5.0f;
};
```

**player.cpp:**
```cpp
#include "player.hpp"
#include <iostream>
#include <cmath>

Player::Player() = default;

void Player::setup() {
    std::cout << "Player (C++): Setup\n";
    
    if (auto* engine = get_engine()) {
        auto& rm = engine->get_resource_manager();
        jump_sound_ = std::dynamic_pointer_cast<nathan::AudioResource>(
            rm.load("assets/audio/jump.wav"));
    }
}

void Player::loop(float delta) {
    x_ += delta * speed_;
    
    // Simulate jumping every 2 units
    if (std::fmod(x_, 10.0f) < 0.1f) {
        jump();
    }
}

void Player::jump() {
    std::cout << "Player jumped!\n";
    if (jump_sound_) {
        jump_sound_->play();
    }
}
```

---

*This document provides a complete reference for implementing minigame loading in the NATHAN Game Engine. After completing the Resource & Config Management system, you can use this document to implement minigame support based on your chosen approach.*
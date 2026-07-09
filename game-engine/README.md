# NATHAN Game Engine - Get Started

This folder contains the game engine for the NATHAN console project. It provides a simple foundation for building audio-based games for blind and visually impaired players.

## What's Inside

The game engine is organized into two main parts:

- **gameplay_foundations/** - Core engine code (Engine, NodePool, Node)
- **demo/** - Example game demonstrating all features

## Prerequisites

Before you start, make sure you have the following installed:

- **CMake** 3.10+ (for project configuration)
- **GCC** (C++20 compiler)
- **Make** (build system)
- **GDB** (optional, for debugging)

On **Linux** (Debian/Ubuntu):
```bash
sudo apt install cmake g++ make gdb
```

On **macOS** (with Homebrew):
```bash
brew install cmake make gdb
```

On **Windows**: Should work with **MinGW** or **MSYS2** (not officially tested yet).

## Quick Start

### 1. Build the project

Run these commands from the game-engine directory:

```bash
cmake -S . -B build
cmake --build build
```

### 2. Run the demo

```bash
./build/demo_game
```

This will run a demo game that shows:
- Scene switching (MainMenu → MiniGame)
- Node creation and lifecycle
- Event system (collision detection, score updates)
- Automatic setup and cleanup

## Creating Your First Game

### Create a custom node

All game entities are **Nodes**. Create your own by inheriting from the Node class:

```cpp
// my_node.hpp
#include "gameplay_foundations/node/node.hpp"

class MyNode : public Node {
public:
    void setup() override;    // Called when node is created
    void loop(float delta) override;  // Called every frame
    void cleanup() override;  // Called when node is destroyed
};
```

```cpp
// my_node.cpp
#include "my_node.hpp"
#include <iostream>

void MyNode::setup() {
    std::cout << "MyNode is ready!\n";
}

void MyNode::loop(float delta) {
    // delta = time since last frame (in seconds)
    // Put your update logic here
}

void MyNode::cleanup() {
    std::cout << "MyNode is being removed\n";
}
```

### Build a scene

A scene is just a Node with children:

```cpp
// my_scene.hpp
#include "gameplay_foundations/node/node.hpp"

class MyScene : public Node {
public:
    void setup() override;
};
```

```cpp
// my_scene.cpp
#include "my_scene.hpp"
#include "my_node.hpp"

void MyScene::setup() {
    // Create and add a node to the scene
    auto my_node = std::make_unique<MyNode>();
    my_node->name = "my_node";
    add_child(std::move(my_node));
}
```

### Set your scene as the root

```cpp
// main.cpp
#include "gameplay_foundations/engine/engine.hpp"
#include "my_scene.hpp"

int main() {
    Engine::instance().set_root(std::make_unique<MyScene>());
    Engine::instance().run();
    return 0;
}
```

## Key Concepts

### Scenes
Your game is organized into **scenes** - different screens like main menus, gameplay levels, etc. Switch between them using `SceneManager`.

### Nodes
Everything in your game is a **Node** - players, enemies, UI elements, etc. Nodes form a tree structure where each node can have children.

### Events
Nodes can **emit** events (like "collision" or "score_update") and **listen** for events from other nodes. This is how game elements communicate.

### Lifecycle
- **setup()** - Called automatically when a node is created
- **loop(delta)** - Called automatically every frame
- **cleanup()** - Called automatically when a node is destroyed

You rarely need to call these manually.

## Next Steps

1. **Explore the demo** - Look at `demo/scenes/` to see how the example game is structured
2. **Create your own nodes** - Start with simple entities and build up
3. **Use events** - Try emitting and listening to custom events
4. **Build a complete game** - Combine scenes, nodes, and events

For more technical details, see the documentation in `gameplay_foundations/README.md`.

# Game Engine - Gameplay Foundations

The core systems of the game engine: **Engine**, **NodePool**, and **Node** hierarchy.

## Architecture Overview

### Engine (Singleton)

The central manager. Only one instance exists (accessed via `Engine::instance()`).

```cpp
// Access the singleton
Engine& engine = Engine::instance();

// Set the root scene
engine.set_root(std::make_unique<MyScene>());

// Start the game loop
engine.run();

// Stop the game loop
engine.stop();
```

**Key responsibilities:**
- Owns the `NodePool` (all nodes are stored here)
- Manages the game loop with fixed timestep
- Processes node creation/destruction

### NodePool

Manages the lifecycle of all `Node` objects. Owned by `Engine`.

**Automatic behavior:**
- When a node is added via `NodePool::create()`, `setup()` is called automatically
- When a node is destroyed via `NodePool::destroy()`, `cleanup()` is called automatically
- Destroyed nodes are removed from the pool on the next frame

### Node (Base Class)

All game entities inherit from `Node`. Forms a tree structure (scene graph).

```cpp
class MyNode : public Node {
public:
    void setup() override {
        // Called automatically when node is created
    }
    
    void loop(float delta) override {
        // Called every frame
    }
    
    void cleanup() override {
        // Called automatically when node is destroyed
    }
};
```

**Tree structure:**
- `parent` - Pointer to parent node (nullptr for root)
- `children` - Vector of child node pointers
- `add_child(std::unique_ptr<Node>)` - Adds a child and transfers ownership to NodePool
- `remove_child(Node*)` - Removes a child and marks it for deletion
- `destroy()` - Removes self from parent and marks for deletion

### Scene Graph Example

```cpp
class Player : public Node {
public:
    void setup() override {
        std::cout << "Player created!\n";
    }
    void loop(float delta) override {
        x += speed * delta;
    }
    float x = 0, speed = 5.0f;
};

class GameScene : public Node {
public:
    void setup() override {
        // Create player and add to tree
        auto player = std::make_unique<Player>();
        player->name = "player";
        add_child(std::move(player));
    }
};

// In main
Engine::instance().set_root(std::make_unique<GameScene>());
Engine::instance().run();
```

## Lifecycle

1. **Creation**: `NodePool::create(std::unique_ptr<Node>)` → calls `setup()` automatically
2. **Update**: `Engine::run()` → calls `loop(delta)` on all active nodes every frame
3. **Destruction**: `NodePool::destroy(Node*)` → calls `cleanup()` automatically, then removes on next frame

## Scene Management

Use `SceneManager` to switch between scenes. Each scene is a `Node` tree.

```cpp
// In your scene
void MyScene::start_next_scene() {
    if (auto* manager = dynamic_cast<SceneManager*>(parent)) {
        manager->switch_to_scene(std::make_unique<NextScene>());
    }
}
```

## Best Practices

1. **Always use `add_child(std::move(node))`** - This transfers ownership to NodePool
2. **Don't manually call `setup()` or `cleanup()`** - These are automatic
3. **Use `destroy()` for removal** - Marks node for deferred deletion (safe during traversal)
4. **All nodes must have unique names** - Used for lookups and debugging
5. **NodePool is per-Engine** - Since Engine is singleton, there's one global pool

## Event System (Optional)

Nodes can emit and subscribe to typed events. See `event_node.hpp` for details.

```cpp
// Subscribe to an event
on<CollisionEvent>("collision", [this](const CollisionEvent& e) {
    std::cout << "Hit: " << e.force << "\n";
});

// Emit an event (bubbles up to parent)
emit<CollisionEvent>("collision", {"enemy", 5.0f});
```

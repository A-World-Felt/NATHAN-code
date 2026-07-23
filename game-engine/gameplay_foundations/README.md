# Game Engine - Gameplay Foundations

The core systems of the game engine: **Engine**, **NodePool**, and **Node** hierarchy.

## Architecture Overview

### Engine

The central manager. Uses dependency injection - **not a singleton**. Create an instance per game.

```cpp
// Create an engine instance
Engine engine;

// Set the root scene
engine.set_root(std::make_unique<MyScene>());

// Start the game loop
engine.run();

// Stop the game loop
engine.stop();
```

**Key responsibilities:**
- Owns the `NodePool` (all nodes are stored here)
- Owns the `EventBus` (for global event handling)
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
- `add_child(std::unique_ptr<Node>)` - Adds a child and transfers ownership to NodePool; **child automatically inherits Engine from parent**
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
        player->set_name("player");
        add_child(std::move(player));
        // Player automatically inherits Engine from GameScene
    }
};

// In main
nathan::Engine engine;
engine.set_root(std::make_unique<GameScene>());
engine.run();
```

## Lifecycle

1. **Creation**: `NodePool::create(std::unique_ptr<Node>)` → calls `setup()` automatically
2. **Update**: `Engine::run()` → calls `loop(delta)` on all active nodes every frame
3. **Destruction**: `NodePool::destroy(Node*)` → calls `cleanup()` automatically, then removes on next frame

## Scene Management

Use `SceneManager` to switch between scenes. Each scene is a `Node` tree. When switching scenes, `SceneManager` automatically sets the Engine on the new scene before adding it as a child.

```cpp
// In your scene
void MyScene::start_next_scene() {
    if (auto* manager = dynamic_cast<SceneManager*>(get_parent())) {
        manager->switch_to_scene(std::make_unique<NextScene>());
    }
}
```

## Best Practices

1. **Always use `add_child(std::move(node))`** - This transfers ownership to NodePool
2. **Don't manually call `setup()` or `cleanup()`** - These are automatic
3. **Use `destroy()` for removal** - Marks node for deferred deletion (safe during traversal)
4. **All nodes must have unique names** - Use `set_name()`; used for lookups and debugging
5. **NodePool and EventBus are per-Engine** - Each Engine instance has its own NodePool and EventBus
6. **Prefer parent inheritance** for Engine - use `add_child()` and let children inherit Engine automatically

## Event System

Use `EventNode` (inherits from Node + EventEmitter) for nodes that need to emit or listen to events. Events are type-safe and can carry custom data.

**Event types** are any struct/class you define:

```cpp
struct CollisionEvent {
    std::string with;
    float force;
};
```

**Subscribe to events:**
```cpp
// In an EventNode
on<CollisionEvent>("collision", [this](const CollisionEvent& e) {
    std::cout << "Hit: " << e.with << " with force " << e.force << "\n";
});

// Subscribe once (auto-disconnects after first call)
once<ScoreEvent>("score", [this](const ScoreEvent& e) {
    std::cout << "First score: " << e.points << "\n";
});
```

**Emit events:**
```cpp
// Bubbles up to parent (and parent's parent, etc.)
emit<CollisionEvent>("collision", {"enemy", 5.0f});

// Emit to children only
emit_to_children<ScoreEvent>("score", {100, "coin"});
```

**Disconnect:**
```cpp
// Disconnect a specific subscription
ConnectionToken token = on<...>(...);
off(token);

// Disconnect all subscriptions for an event type
off("collision");

// Disconnect all subscriptions
off_all();
```

See `event_node.hpp` and `event_emitter.hpp` for implementation details.

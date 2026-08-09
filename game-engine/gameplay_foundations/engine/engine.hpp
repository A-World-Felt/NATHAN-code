#ifndef GAME_ENGINE_ENGINE_ENGINE_H_
#define GAME_ENGINE_ENGINE_ENGINE_H_

#include "audio/soundscape.hpp"
#include "engine/node_pool.hpp"
#include "events/event_bus.hpp"
#include "inputs/iinput_device.hpp"

namespace nathan {

// Forward declaration
class Node;

// Engine manages the game world and its subsystems
class Engine {
public:
    // Delete copy/move constructors and assignment
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    Engine();

    void set_root(std::unique_ptr<Node> scene);
    void run();
    void stop();

    // Get current root
    Node* get_root() const { return root_; }

    // Access to NodePool
    NodePool& get_node_pool() { return node_pool_; }
    const NodePool& get_node_pool() const { return node_pool_; }

    // Access to EventBus
    EventBus& get_event_bus() { return event_bus_; }
    const EventBus& get_event_bus() const { return event_bus_; }

    // Access to Soundscape
    Soundscape& get_soundscape() { return soundscape_; }
    const Soundscape& get_soundscape() const { return soundscape_; }

private:
    Node* root_;  // Raw pointer - owned by node_pool_
    NodePool node_pool_;  // Owns all nodes
    EventBus event_bus_;   // Owns the event bus
    Soundscape soundscape_; // Owns the soundscape
    std::shared_ptr<IInputDevice> input_device_;
    bool running_;
};

}  // namespace nathan

#endif  // GAME_ENGINE_ENGINE_ENGINE_H_
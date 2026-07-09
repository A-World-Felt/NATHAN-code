#include "engine.hpp"
#include "node_pool.hpp"
#include "../node/node.hpp"

#include <chrono>
#include <iostream>
#include <thread>

// Singleton instance definition
Engine* Engine::instance_ = nullptr;

Engine::Engine() : root(nullptr), running(true) {
    // NodePool is initialized as a member
}

Engine& Engine::instance() {
    if (!instance_) {
        instance_ = new Engine();
    }
    return *instance_;
}

void Engine::set_root(std::unique_ptr<Node> scene) {
    root = scene.get();
    if (scene) {
        node_pool_.create(std::move(scene));
        // setup() is called automatically by NodePool::create()
    }
}

void Engine::stop() {
    running = false;
}

void Engine::run() {
    if (!root) return;

    std::cout << "[Engine] Starting game loop...\n";

    using clock = std::chrono::high_resolution_clock;

    constexpr float fixed_dt = 1.0f / 60.0f; // 60 FPS physics
    float accumulator = 0.0f;

    auto previous = clock::now();

    while (running) {
        auto now = clock::now();
        float frame_time =
            std::chrono::duration<float>(now - previous).count();

        previous = now;

        // Safety clamp (prevents debugger spikes)
        if (frame_time > 0.25f)
            frame_time = 0.25f;

        accumulator += frame_time;

        // Process destroyed nodes BEFORE traversal
        node_pool_.cleanup_destroyed();

        // FIXED UPDATE LOOP - iterate all nodes in pool
        while (accumulator >= fixed_dt) {
            // Loop through all active nodes
            for (auto& node_ptr : node_pool_.get_pool()) {
                if (!node_ptr->destroyed) {
                    node_ptr->loop(fixed_dt);
                }
            }
            accumulator -= fixed_dt;
        }

        // Optional: prevent 100% CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

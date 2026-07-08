#include "engine.hpp"
#include "tree_traverser.hpp"
#include "../node/node.hpp"

#include <chrono>
#include <thread>

void Engine::set_root(std::unique_ptr<Node> scene) {
    root = std::move(scene);
}

void Engine::stop() {
    running = false;
}

void Engine::run() {
    if (!root) return;

    // Setup entire tree using iterative traversal - automatic for all nodes
    TreeTraverser::traverse_setup(root.get());

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

        // FIXED UPDATE LOOP - automatic for all nodes using iterative traversal
        while (accumulator >= fixed_dt) {
            TreeTraverser::traverse_loop(root.get(), fixed_dt);
            accumulator -= fixed_dt;
        }

        // Optional: prevent 100% CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

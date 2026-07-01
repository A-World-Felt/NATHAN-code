#include "engine.hpp"
#include "node.hpp"

#include <chrono>
#include <thread>

void Engine::set_root(std::unique_ptr<Node> scene) {
    root = std::move(scene);
}

void Engine::run() {
    if (!root) return;

    root->setup();

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

        // FIXED UPDATE LOOP
        while (accumulator >= fixed_dt) {
            root->loop(fixed_dt); // treat loop as FIXED STEP
            accumulator -= fixed_dt;
        }

        // Optional: prevent 100% CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
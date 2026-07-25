#include "engine/engine.hpp"
#include "scenes/scene_manager.hpp"

int main() {
    // Create Engine instance
    nathan::Engine engine;
    engine.set_root(std::make_unique<nathan::DemoSceneManager>());
    
    engine.run();
    return 0;
}

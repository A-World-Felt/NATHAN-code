#include "engine/engine.hpp"
#include "scenes/scene_manager.hpp"

int main() {
    // Engine is a singleton - use Engine::instance()
    nathan::Engine::instance().set_root(std::make_unique<nathan::DemoSceneManager>());
    nathan::Engine::instance().run();
    return 0;
}

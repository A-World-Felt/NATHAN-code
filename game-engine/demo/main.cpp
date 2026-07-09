#include "engine/engine.hpp"
#include "scenes/scene_manager.hpp"

int main() {
    // Engine is a singleton - use Engine::instance()
    Engine::instance().set_root(std::make_unique<SceneManager>());
    Engine::instance().run();
    return 0;
}

#include "engine/engine.hpp"
#include "main-scene.hpp"

int main() {
    Engine engine;
    engine.set_root(std::make_unique<MainScene>());
    engine.run();
    return 0;
}
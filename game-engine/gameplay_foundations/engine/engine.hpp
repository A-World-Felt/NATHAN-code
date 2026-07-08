#pragma once

#include <memory>

// Forward declarations
class Node;

class Engine {
public:
    void set_root(std::unique_ptr<Node> scene);
    void run();
    void stop();

private:
    std::unique_ptr<Node> root;
    bool running = true;
};

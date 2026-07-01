#pragma once

#include <memory>

class Node;

class Engine {
public:
    void set_root(std::unique_ptr<Node> scene);
    void run();

private:
    std::unique_ptr<Node> root;

    bool running = true;
};
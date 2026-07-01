#pragma once

#include <string>
#include <vector>
#include <memory>

class Node {
public:
    std::string name;
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;

    virtual ~Node() = default;

    virtual void setup();
    virtual void loop(float delta);

    void add_child(std::unique_ptr<Node> child);
    void remove_child(const std::string& name);
};
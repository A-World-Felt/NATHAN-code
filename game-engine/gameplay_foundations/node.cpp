#include "node.hpp"
#include <algorithm>

void Node::setup() {}

void Node::loop(float delta) {}

void Node::add_child(std::unique_ptr<Node> child) {
    child->parent = this;
    children.push_back(std::move(child));
}

void Node::remove_child(const std::string& name) {
    children.erase(
        std::remove_if(children.begin(), children.end(),
            [&name](const std::unique_ptr<Node>& child) {
                return child->name == name;
            }),
        children.end()
    );
}
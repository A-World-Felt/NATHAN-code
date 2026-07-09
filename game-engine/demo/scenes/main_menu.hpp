#pragma once

#include "node/event_node.hpp"
#include <iostream>

class MainMenu : public EventNode {
public:
    void setup() override;
    void loop(float delta) override;
    void cleanup() override;
};

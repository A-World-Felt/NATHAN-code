#ifndef GAME_ENGINE_DEMO_SCENES_MAIN_MENU_H_
#define GAME_ENGINE_DEMO_SCENES_MAIN_MENU_H_

#include "node/event_node.hpp"

namespace nathan {

class MainMenu : public EventNode {
public:
    void setup() override;
    void loop(float delta) override;
    void cleanup() override;
};

}  // namespace nathan

#endif  // GAME_ENGINE_DEMO_SCENES_MAIN_MENU_H_

// Unit tests for SceneManager using Google Test

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "gameplay_foundations/node/scene_manager.hpp"
#include "gameplay_foundations/node/node.hpp"

namespace nathan {

class TestScene : public Node {
public:
    bool setup_called = false;
    bool cleanup_called = false;
    
    void setup() override { setup_called = true; }
    void cleanup() override { cleanup_called = true; }
};

} // namespace nathan

TEST(SceneManagerTest, DefaultConstruction) {
    nathan::SceneManager manager;
    
    EXPECT_EQ(manager.get_name(), "SceneManager");
    EXPECT_EQ(manager.get_current_scene(), nullptr);
}

TEST(SceneManagerTest, CurrentSceneAccessor) {
    nathan::SceneManager manager;
    
    // Initially no scene
    EXPECT_EQ(manager.get_current_scene(), nullptr);
}

// Note: Full switch_to_scene tests require Engine singleton
// These are tested in integration tests

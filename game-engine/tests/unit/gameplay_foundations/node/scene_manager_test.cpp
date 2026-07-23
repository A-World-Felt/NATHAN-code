// Unit tests for SceneManager using Google Test and Google Mock

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "gameplay_foundations/node/scene_manager.hpp"
#include "gameplay_foundations/node/node.hpp"

namespace nathan {

class MockScene : public Node {
public:
    MOCK_METHOD(void, setup, (), (override));
    MOCK_METHOD(void, loop, (float delta), (override));
    MOCK_METHOD(void, cleanup, (), (override));
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

TEST(SceneManagerTest, SwitchToSceneSetsCurrentScene) {
    nathan::Engine engine;
    nathan::SceneManager manager;
    manager.set_engine(&engine);
    
    auto scene = std::make_unique<nathan::MockScene>();
    manager.switch_to_scene(std::move(scene));
    
    EXPECT_NE(manager.get_current_scene(), nullptr);
}

TEST(SceneManagerTest, SwitchToSceneCallsSetupOnNewScene) {
    nathan::Engine engine;
    nathan::SceneManager manager;
    manager.set_engine(&engine);
    
    auto scene = std::make_unique<nathan::MockScene>();
    nathan::MockScene* raw_scene = scene.get();
    EXPECT_CALL(*raw_scene, setup()).Times(1);
    
    manager.switch_to_scene(std::move(scene));
}

TEST(SceneManagerTest, SwitchToSceneSetsEngineOnNewScene) {
    nathan::Engine engine;
    nathan::SceneManager manager;
    manager.set_engine(&engine);
    
    auto scene = std::make_unique<nathan::MockScene>();
    manager.switch_to_scene(std::move(scene));
    
    EXPECT_EQ(manager.get_current_scene()->get_engine(), &engine);
}

TEST(SceneManagerTest, SwitchToSceneCleansUpPreviousScene) {
    nathan::Engine engine;
    nathan::SceneManager manager;
    manager.set_engine(&engine);
    
    auto scene1 = std::make_unique<nathan::MockScene>();
    nathan::MockScene* raw_scene1 = scene1.get();
    EXPECT_CALL(*raw_scene1, cleanup()).Times(1);
    
    manager.switch_to_scene(std::move(scene1));
    
    auto scene2 = std::make_unique<nathan::MockScene>();
    nathan::MockScene* raw_scene2 = scene2.get();
    manager.switch_to_scene(std::move(scene2));
    
    // scene2 should now be the current scene
    EXPECT_EQ(manager.get_current_scene(), raw_scene2);
}

TEST(SceneManagerTest, SwitchToSceneWithoutEngineDoesNotCrash) {
    nathan::SceneManager manager;
    // No engine set - should handle gracefully
    
    auto scene = std::make_unique<nathan::MockScene>();
    manager.switch_to_scene(std::move(scene));
    
    // Scene should still be set even without engine
    EXPECT_NE(manager.get_current_scene(), nullptr);
}

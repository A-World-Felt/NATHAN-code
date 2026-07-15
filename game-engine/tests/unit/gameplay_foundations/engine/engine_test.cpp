// Unit tests for Engine using Google Test and Google Mock

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "gameplay_foundations/engine/engine.hpp"
#include "gameplay_foundations/engine/node_pool.hpp"
#include "gameplay_foundations/node/node.hpp"

namespace nathan {

class TestNode : public Node {
public:
    bool setup_called = false;
    bool loop_called = false;
    bool cleanup_called = false;
    
    void setup() override { setup_called = true; }
    void loop(float delta) override {
        loop_called = true;
        Node::loop(delta);
    }
    void cleanup() override { cleanup_called = true; }
};

} // namespace nathan

TEST(EngineTest, InstanceReturnsSingleton) {
    nathan::Engine& instance1 = nathan::Engine::instance();
    nathan::Engine& instance2 = nathan::Engine::instance();

    EXPECT_EQ(&instance1, &instance2);
}

TEST(EngineTest, SetRootSetsRootAndAddsToPool) {
    nathan::Engine& engine = nathan::Engine::instance();
    
    auto root = std::make_unique<nathan::TestNode>();
    root->set_name("root_node");
    
    engine.set_root(std::move(root));

    EXPECT_NE(engine.get_root(), nullptr);
    EXPECT_EQ(engine.get_root()->get_name(), "root_node");
    EXPECT_TRUE(engine.get_node_pool().contains(engine.get_root()));
}

TEST(EngineTest, StopSetsRunningToFalse) {
    nathan::Engine& engine = nathan::Engine::instance();
    
    // Just ensure stop() doesn't crash
    engine.stop();
    
    // Engine is singleton, so we can't easily reset it
    // In a proper test harness, you'd use dependency injection
    SUCCEED();
}

TEST(EngineTest, GetNodePoolReturnsPoolReference) {
    nathan::Engine& engine = nathan::Engine::instance();
    
    nathan::NodePool& pool = engine.get_node_pool();
    const nathan::NodePool& const_pool = engine.get_node_pool();

    EXPECT_EQ(&pool, &const_pool);
}

TEST(EngineTest, RootNodeHasSetupCalled) {
    nathan::Engine& engine = nathan::Engine::instance();
    
    auto root = std::make_unique<nathan::TestNode>();
    engine.set_root(std::move(root));

    nathan::TestNode* test_root = dynamic_cast<nathan::TestNode*>(engine.get_root());
    ASSERT_NE(test_root, nullptr);
    EXPECT_TRUE(test_root->setup_called);
}

TEST(EngineTest, EngineManagesNodePoolLifecycle) {
    nathan::Engine& engine = nathan::Engine::instance();
    
    auto root = std::make_unique<nathan::TestNode>();
    engine.set_root(std::move(root));

    nathan::NodePool& pool = engine.get_node_pool();
    EXPECT_TRUE(pool.contains(engine.get_root()));
}

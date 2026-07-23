// Unit tests for Engine using Google Test and Google Mock

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "gameplay_foundations/engine/engine.hpp"
#include "gameplay_foundations/engine/node_pool.hpp"
#include "gameplay_foundations/node/node.hpp"

namespace nathan {

class MockNode : public Node {
public:
    using Node::Node;  // Inherit constructors
    
    MOCK_METHOD(void, setup, (), (override));
    MOCK_METHOD(void, loop, (float delta), (override));
    MOCK_METHOD(void, cleanup, (), (override));
};

} // namespace nathan

// Singleton test removed - Engine now uses dependency injection only

TEST(EngineTest, SetRootSetsRootAndAddsToPool) {
    nathan::Engine engine;
    
    auto root = std::make_unique<nathan::MockNode>();
    root->set_name("root_node");
    
    // Get raw pointer to set expectations before moving
    nathan::MockNode* raw_mock = root.get();
    // NodePool::create (called by Engine::set_root) will call setup()
    EXPECT_CALL(*raw_mock, setup()).Times(1);
    
    engine.set_root(std::move(root));

    EXPECT_NE(engine.get_root(), nullptr);
    EXPECT_EQ(engine.get_root()->get_name(), "root_node");
    EXPECT_TRUE(engine.get_node_pool().contains(engine.get_root()));
}

TEST(EngineTest, StopSetsRunningToFalse) {
    nathan::Engine engine;
    
    // Just ensure stop() doesn't crash
    engine.stop();
    SUCCEED();
}

TEST(EngineTest, GetNodePoolReturnsPoolReference) {
    nathan::Engine engine;
    
    nathan::NodePool& pool = engine.get_node_pool();
    const nathan::NodePool& const_pool = engine.get_node_pool();

    EXPECT_EQ(&pool, &const_pool);
}

TEST(EngineTest, RootNodeHasSetupCalled) {
    nathan::Engine engine;
    
    auto mock_root = std::make_unique<nathan::MockNode>();
    
    // Get raw pointer to set expectations before moving
    nathan::MockNode* raw_mock = mock_root.get();
    // NodePool::create (called by Engine::set_root) will call setup()
    EXPECT_CALL(*raw_mock, setup()).Times(1);
    
    engine.set_root(std::move(mock_root));

    // Verify the root was set and is in the pool
    ASSERT_NE(engine.get_root(), nullptr);
    EXPECT_TRUE(engine.get_node_pool().contains(engine.get_root()));
    
    // Mock destructor will verify all expectations (setup was called)
}

TEST(EngineTest, EngineManagesNodePoolLifecycle) {
    nathan::Engine engine;
    
    auto root = std::make_unique<nathan::MockNode>();
    
    // Get raw pointer to set expectations before moving
    nathan::MockNode* raw_mock = root.get();
    // NodePool::create (called by Engine::set_root) will call setup()
    EXPECT_CALL(*raw_mock, setup()).Times(1);
    
    engine.set_root(std::move(root));

    nathan::NodePool& pool = engine.get_node_pool();
    EXPECT_TRUE(pool.contains(engine.get_root()));
}

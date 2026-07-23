// Unit tests for NodePool using Google Test and Google Mock

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

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

class MockChildNode : public MockNode {
public:
    using MockNode::MockNode;
};

} // namespace nathan

TEST(NodePoolTest, CreateAddsNodeToPool) {
    nathan::NodePool pool;
    auto node = std::make_unique<nathan::Node>();
    node->set_name("test_node");

    nathan::Node* raw = pool.create(std::move(node));

    ASSERT_NE(raw, nullptr);
    EXPECT_EQ(raw->get_name(), "test_node");
}

TEST(NodePoolTest, CreateCallsSetupOnNode) {
    nathan::NodePool pool;
    auto mock_node = std::make_unique<nathan::MockNode>();

    // Get raw pointer to set expectations before moving
    nathan::MockNode* raw_mock = mock_node.get();
    // NodePool::create will call setup()
    EXPECT_CALL(*raw_mock, setup()).Times(1);

    nathan::Node* raw = pool.create(std::move(mock_node));

    ASSERT_NE(raw, nullptr);
    // Mock destructor verifies setup was called
}

TEST(NodePoolTest, DestroyCallsCleanupOnNode) {
    nathan::NodePool pool;
    auto mock_node = std::make_unique<nathan::MockNode>();
    
    // Get raw pointer to set expectations before moving
    nathan::MockNode* raw_mock = mock_node.get();
    // NodePool::create calls setup()
    EXPECT_CALL(*raw_mock, setup()).Times(1);
    
    nathan::Node* raw = pool.create(std::move(mock_node));

    // Expect cleanup to be called exactly once
    EXPECT_CALL(*raw_mock, cleanup()).Times(1);

    pool.destroy(raw);

    EXPECT_TRUE(raw->is_destroyed());
    // Mock destructor verifies both setup and cleanup were called
}

TEST(NodePoolTest, CleanupDestroyedRemovesDestroyedNodes) {
    nathan::NodePool pool;
    auto node = std::make_unique<nathan::Node>();
    nathan::Node* raw = pool.create(std::move(node));

    EXPECT_TRUE(pool.contains(raw));

    pool.destroy(raw);
    pool.cleanup_destroyed();

    EXPECT_FALSE(pool.contains(raw));
}

TEST(NodePoolTest, CleanupDestroyedPreservesActiveNodes) {
    nathan::NodePool pool;
    auto node1 = std::make_unique<nathan::Node>();
    auto node2 = std::make_unique<nathan::Node>();
    
    nathan::Node* raw1 = pool.create(std::move(node1));
    nathan::Node* raw2 = pool.create(std::move(node2));

    pool.destroy(raw1);
    pool.cleanup_destroyed();

    EXPECT_FALSE(pool.contains(raw1));
    EXPECT_TRUE(pool.contains(raw2));
}

TEST(NodePoolTest, QueueRemoveIsAliasForDestroy) {
    nathan::NodePool pool;
    auto mock_node = std::make_unique<nathan::MockNode>();
    
    // Get raw pointer to set expectations before moving
    nathan::MockNode* raw_mock = mock_node.get();
    // NodePool::create calls setup()
    EXPECT_CALL(*raw_mock, setup()).Times(1);
    
    nathan::Node* raw = pool.create(std::move(mock_node));

    // Expect cleanup to be called exactly once
    EXPECT_CALL(*raw_mock, cleanup()).Times(1);

    pool.queue_remove(raw);

    EXPECT_TRUE(raw->is_destroyed());
    // Mock destructor verifies both setup and cleanup were called
}

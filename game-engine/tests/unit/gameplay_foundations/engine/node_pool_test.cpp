// Unit tests for NodePool using Google Test

#include <gtest/gtest.h>
#include <memory>

#include "gameplay_foundations/engine/node_pool.hpp"
#include "gameplay_foundations/node/node.hpp"

namespace nathan {

class TrackedNode : public Node {
public:
    bool setup_called = false;
    bool cleanup_called = false;

    void setup() override { setup_called = true; }
    void cleanup() override { cleanup_called = true; }
};

class ChildTrackedNode : public TrackedNode {
public:
    bool child_setup_called = false;
    bool child_cleanup_called = false;

    void setup() override {
        TrackedNode::setup();
        child_setup_called = true;
    }
    void cleanup() override {
        TrackedNode::cleanup();
        child_cleanup_called = true;
    }
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
    auto tracked_node = std::make_unique<nathan::TrackedNode>();

    nathan::Node* raw = pool.create(std::move(tracked_node));

    ASSERT_NE(raw, nullptr);
    nathan::TrackedNode* tracked = dynamic_cast<nathan::TrackedNode*>(raw);
    ASSERT_NE(tracked, nullptr);
    EXPECT_TRUE(tracked->setup_called);
}

TEST(NodePoolTest, DestroyCallsCleanupOnNode) {
    nathan::NodePool pool;
    auto tracked_node = std::make_unique<nathan::TrackedNode>();
    nathan::Node* raw = pool.create(std::move(tracked_node));

    pool.destroy(raw);

    nathan::TrackedNode* tracked = dynamic_cast<nathan::TrackedNode*>(raw);
    ASSERT_NE(tracked, nullptr);
    EXPECT_TRUE(tracked->cleanup_called);
    EXPECT_TRUE(raw->is_destroyed());
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
    auto tracked_node = std::make_unique<nathan::TrackedNode>();
    nathan::Node* raw = pool.create(std::move(tracked_node));

    pool.queue_remove(raw);

    nathan::TrackedNode* tracked = dynamic_cast<nathan::TrackedNode*>(raw);
    ASSERT_NE(tracked, nullptr);
    EXPECT_TRUE(tracked->cleanup_called);
    EXPECT_TRUE(raw->is_destroyed());
}

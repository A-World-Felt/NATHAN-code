// Unit tests for Node using Google Test

#include <gtest/gtest.h>
#include <memory>

#include "gameplay_foundations/node/node.hpp"

namespace nathan {

class TestNode : public Node {
public:
    bool custom_setup_called = false;
    bool custom_loop_called = false;
    bool custom_cleanup_called = false;
    
    void setup() override { custom_setup_called = true; }
    void loop(float delta) override { custom_loop_called = true; }
    void cleanup() override { custom_cleanup_called = true; }
};

} // namespace nathan

TEST(NodeTest, DefaultLifecycleMethodsDoNothing) {
    nathan::Node node;
    
    // These should not crash
    node.setup();
    node.loop(0.016f);
    node.cleanup();
    
    SUCCEED();
}

TEST(NodeTest, NameAccessorsWork) {
    nathan::Node node;
    
    node.set_name("test_node");
    EXPECT_EQ(node.get_name(), "test_node");
}

TEST(NodeTest, ParentAccessorsWork) {
    nathan::Node node;
    nathan::Node parent;
    parent.set_name("parent");
    
    node.set_parent(&parent);
    EXPECT_EQ(node.get_parent(), &parent);
}

TEST(NodeTest, DestroyedFlagWorks) {
    nathan::Node node;
    
    EXPECT_FALSE(node.is_destroyed());
    node.set_destroyed(true);
    EXPECT_TRUE(node.is_destroyed());
    
    // Setting again should have no effect
    node.set_destroyed(true);
    EXPECT_TRUE(node.is_destroyed());
}

TEST(NodeTest, ChildrenVectorAccessor) {
    nathan::Node parent;
    
    EXPECT_TRUE(parent.get_children().empty());
    EXPECT_EQ(parent.get_children().size(), 0);
}

TEST(NodeTest, NameDefaultsToEmpty) {
    nathan::Node node;
    EXPECT_TRUE(node.get_name().empty());
}

TEST(NodeTest, ParentDefaultsToNullptr) {
    nathan::Node node;
    EXPECT_EQ(node.get_parent(), nullptr);
}

TEST(NodeTest, DestroyedDefaultsToFalse) {
    nathan::Node node;
    EXPECT_FALSE(node.is_destroyed());
}

TEST(NodeTest, FactoryMethodCreatesInstance) {
    auto node = nathan::Node::create<nathan::TestNode>();
    
    EXPECT_NE(node, nullptr);
    EXPECT_NE(dynamic_cast<nathan::TestNode*>(node.get()), nullptr);
}

TEST(NodeTest, OverrideLifecycleMethods) {
    nathan::TestNode test_node;
    
    test_node.setup();
    EXPECT_TRUE(test_node.custom_setup_called);
    
    test_node.loop(0.016f);
    EXPECT_TRUE(test_node.custom_loop_called);
    
    test_node.cleanup();
    EXPECT_TRUE(test_node.custom_cleanup_called);
}

TEST(NodeTest, ParentChildRelationship) {
    nathan::Node parent;
    parent.set_name("parent");
    
    nathan::Node child;
    child.set_name("child");
    
    child.set_parent(&parent);
    EXPECT_EQ(child.get_parent(), &parent);
    EXPECT_TRUE(parent.get_children().empty());
}

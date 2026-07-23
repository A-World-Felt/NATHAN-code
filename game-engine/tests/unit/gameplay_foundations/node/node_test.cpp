// Unit tests for Node using Google Test and Google Mock

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "gameplay_foundations/node/node.hpp"
#include "gameplay_foundations/node/scene_manager.hpp"
#include "gameplay_foundations/engine/engine.hpp"

namespace nathan {
    class NodePool;

class MockNode : public Node {
public:
    MOCK_METHOD(void, setup, (), (override));
    MOCK_METHOD(void, loop, (float delta), (override));
    MOCK_METHOD(void, cleanup, (), (override));
};

} // namespace nathan

// Basic Node tests
TEST(NodeTest, DefaultLifecycleMethodsDoNothing) {
    nathan::Node node;
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

TEST(NodeTest, DefaultConstructorWorks) {
    auto node = std::make_unique<nathan::MockNode>();
    EXPECT_NE(node, nullptr);
}

TEST(NodeTest, OverrideLifecycleMethods) {
    nathan::MockNode mock_node;
    
    EXPECT_CALL(mock_node, setup()).Times(1);
    EXPECT_CALL(mock_node, loop(::testing::_)).Times(1);
    EXPECT_CALL(mock_node, cleanup()).Times(1);
    
    mock_node.setup();
    mock_node.loop(0.016f);
    mock_node.cleanup();
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

// =============================================================================
// Dependency Injection Tests
// =============================================================================

class NodeTestWithEngine : public ::testing::Test {
protected:
    void SetUp() override { engine = new nathan::Engine(); }
    void TearDown() override { delete engine; }
    nathan::Engine* engine = nullptr;
};

TEST_F(NodeTestWithEngine, NodeAcceptsEngineViaSetEngine) {
    nathan::Node node;
    node.set_engine(engine);
    EXPECT_EQ(node.get_engine(), engine);
}

TEST_F(NodeTestWithEngine, NodeDefaultConstructorHasNullEngine) {
    nathan::Node node;
    EXPECT_EQ(node.get_engine(), nullptr);
}

TEST_F(NodeTestWithEngine, AddChildPropagatesEngineToChild) {
    nathan::Node parent;
    parent.set_engine(engine);
    auto child = std::make_unique<nathan::Node>();
    parent.add_child(std::move(child));
    EXPECT_FALSE(parent.get_children().empty());
    EXPECT_EQ(parent.get_children()[0]->get_engine(), engine);
}

TEST_F(NodeTestWithEngine, SetEngineWorks) {
    nathan::Node node;
    EXPECT_EQ(node.get_engine(), nullptr);
    node.set_engine(engine);
    EXPECT_EQ(node.get_engine(), engine);
}

TEST_F(NodeTestWithEngine, DestroyWithoutParentUsesEngine) {
    nathan::Node node;
    node.set_engine(engine);
    node.set_name("test_node");
    auto node_to_add = std::make_unique<nathan::Node>();
    node_to_add->set_engine(engine);
    nathan::Node* raw_ptr = node_to_add.get();
    engine->get_node_pool().create(std::move(node_to_add));
    raw_ptr->destroy();
    EXPECT_TRUE(raw_ptr->is_destroyed());
}

TEST(NodeTest, MultipleEnginesSupported) {
    nathan::Engine engine1;
    nathan::Engine engine2;
    nathan::Node node1;
    nathan::Node node2;
    node1.set_engine(&engine1);
    node2.set_engine(&engine2);
    EXPECT_EQ(node1.get_engine(), &engine1);
    EXPECT_EQ(node2.get_engine(), &engine2);
    EXPECT_NE(node1.get_engine(), node2.get_engine());
}

TEST(NodeTest, GetEngineReturnsCorrectEngine) {
    nathan::Engine engine;
    nathan::Node node;
    node.set_engine(&engine);
    EXPECT_EQ(node.get_engine(), &engine);
}

TEST(NodeTest, ChildInheritsEngineFromParent) {
    nathan::Engine engine;
    nathan::Node parent;
    parent.set_engine(&engine);
    auto child = std::make_unique<nathan::Node>();
    parent.add_child(std::move(child));
    EXPECT_FALSE(parent.get_children().empty());
    EXPECT_EQ(parent.get_children()[0]->get_engine(), &engine);
}

TEST(NodeTest, SceneManagerSetsEngineOnChildren) {
    nathan::Engine engine;
    nathan::SceneManager scene_manager;
    scene_manager.set_engine(&engine);
    auto child_scene = std::make_unique<nathan::Node>();
    child_scene->set_name("child_scene");
    scene_manager.add_child(std::move(child_scene));
    EXPECT_FALSE(scene_manager.get_children().empty());
    EXPECT_EQ(scene_manager.get_children()[0]->get_engine(), &engine);
}

TEST(NodeTest, NodesFromDifferentEnginesAreIndependent) {
    nathan::Engine engine1;
    nathan::Engine engine2;
    nathan::Node parent1;
    nathan::Node parent2;
    parent1.set_engine(&engine1);
    parent2.set_engine(&engine2);
    auto child1 = std::make_unique<nathan::Node>();
    auto child2 = std::make_unique<nathan::Node>();
    parent1.add_child(std::move(child1));
    parent2.add_child(std::move(child2));
    EXPECT_EQ(parent1.get_children()[0]->get_engine(), &engine1);
    EXPECT_EQ(parent2.get_children()[0]->get_engine(), &engine2);
    EXPECT_NE(parent1.get_children()[0]->get_engine(), parent2.get_children()[0]->get_engine());
}

TEST(NodeTest, EnginePropagatesThroughHierarchy) {
    nathan::Engine engine;
    nathan::Node root;
    root.set_engine(&engine);
    auto child1 = std::make_unique<nathan::Node>();
    auto child2 = std::make_unique<nathan::Node>();
    root.add_child(std::move(child1));
    root.get_children()[0]->add_child(std::move(child2));
    EXPECT_EQ(root.get_engine(), &engine);
    EXPECT_EQ(root.get_children()[0]->get_engine(), &engine);
    EXPECT_EQ(root.get_children()[0]->get_children()[0]->get_engine(), &engine);
}

// Unit tests for EventNode using Google Test and Google Mock

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "gameplay_foundations/node/event_node.hpp"
#include "gameplay_foundations/node/node.hpp"

namespace nathan {

struct TestEvent {
    int value;
};

struct AnotherTestEvent {
    std::string message;
};

class MockEventNode : public EventNode {
public:
    using EventNode::EventNode;  // Inherit constructors
    
    MOCK_METHOD(void, setup, (), (override));
    MOCK_METHOD(void, loop, (float delta), (override));
    MOCK_METHOD(void, cleanup, (), (override));
};

} // namespace nathan

TEST(EventNodeTest, DefaultConstruction) {
    nathan::EventNode node;
    
    // EventNode should inherit from Node
    nathan::Node* as_node = &node;
    EXPECT_NE(as_node, nullptr);
}

TEST(EventNodeTest, Lifecycle) {
    nathan::MockEventNode mock_node;
    
    EXPECT_CALL(mock_node, setup()).Times(1);
    EXPECT_CALL(mock_node, cleanup()).Times(1);
    
    mock_node.setup();
    mock_node.cleanup();
}

TEST(EventNodeTest, InheritsNodeFunctionality) {
    nathan::EventNode node;
    
    node.set_name("event_node");
    EXPECT_EQ(node.get_name(), "event_node");
    
    node.set_parent(nullptr);
    EXPECT_EQ(node.get_parent(), nullptr);
    
    EXPECT_FALSE(node.is_destroyed());
}

TEST(EventNodeTest, SubscribeAndEmit) {
    nathan::EventNode node;
    
    bool event_received = false;
    int received_value = 0;
    
    // Subscribe to event
    node.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent& event) {
            event_received = true;
            received_value = event.value;
        }
    );
    
    // Emit event
    nathan::TestEvent event{42};
    node.emit<nathan::TestEvent>("test_event", event);
    
    EXPECT_TRUE(event_received);
    EXPECT_EQ(received_value, 42);
}

TEST(EventNodeTest, MultipleSubscriptions) {
    nathan::EventNode node;
    
    int call_count = 0;
    
    node.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    node.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    nathan::TestEvent event{0};
    node.emit<nathan::TestEvent>("test_event", event);
    
    EXPECT_EQ(call_count, 2);
}

TEST(EventNodeTest, Unsubscribe) {
    nathan::EventNode node;
    
    int call_count = 0;
    
    auto connection = node.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    nathan::TestEvent event{0};
    node.emit<nathan::TestEvent>("test_event", event);
    EXPECT_EQ(call_count, 1);
    
    // Unsubscribe using off(token)
    node.off(connection);
    
    node.emit<nathan::TestEvent>("test_event", event);
    EXPECT_EQ(call_count, 1); // Should not have increased
}

TEST(EventNodeTest, OffAllRemovesAllSubscriptions) {
    nathan::EventNode node;
    
    int call_count = 0;
    
    node.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    node.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    nathan::TestEvent event{0};
    node.emit<nathan::TestEvent>("test_event", event);
    EXPECT_EQ(call_count, 2);
    
    // Remove all
    node.off_all();
    
    node.emit<nathan::TestEvent>("test_event", event);
    EXPECT_EQ(call_count, 2); // Should not have increased
}

TEST(EventNodeTest, DifferentEventTypes) {
    nathan::EventNode node;
    
    bool test_event_received = false;
    bool another_event_received = false;
    
    node.on<nathan::TestEvent>(
        "test",
        [&](const nathan::TestEvent&) { test_event_received = true; }
    );
    
    node.on<nathan::AnotherTestEvent>(
        "another",
        [&](const nathan::AnotherTestEvent&) { another_event_received = true; }
    );
    
    // Emit test event
    nathan::TestEvent test_event{0};
    node.emit<nathan::TestEvent>("test", test_event);
    
    EXPECT_TRUE(test_event_received);
    EXPECT_FALSE(another_event_received);
    
    // Emit another event
    nathan::AnotherTestEvent another_event{"hello"};
    node.emit<nathan::AnotherTestEvent>("another", another_event);
    
    EXPECT_TRUE(another_event_received);
}

TEST(EventNodeTest, ConnectionTokenIsValid) {
    nathan::EventNode node;
    
    auto connection = node.on<nathan::TestEvent>(
        "test",
        [](const nathan::TestEvent&) {});
    
    // Connection token should be valid after creation
    EXPECT_TRUE(connection.is_valid());
    EXPECT_NE(connection.get_emitter(), nullptr);
    EXPECT_NE(connection.get_id(), 0);
}

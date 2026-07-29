// Unit tests for EventEmitter using Google Test

#include <gtest/gtest.h>
#include <string>

#include "gameplay_foundations/events/event_emitter.hpp"

namespace nathan {

struct TestEvent {
    int value;
    std::string name;
};

struct AnotherEvent {
    float data;
};

class TestEmitter : public EventEmitter {
public:
    // Expose protected method for testing
    template<typename T>
    void emit_test(std::string event_type, const T& data) {
        emit_internal<T>(event_type, data);
    }
};

} // namespace nathan

TEST(EventEmitterTest, OnAndEmit) {
    nathan::TestEmitter emitter;
    
    bool called = false;
    int received_value = 0;
    
    auto token = emitter.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent& event) {
            called = true;
            received_value = event.value;
        }
    );
    
    EXPECT_TRUE(token.is_valid());
    
    nathan::TestEvent event{42, "test"};
    emitter.emit_test<nathan::TestEvent>("test_event", event);
    
    EXPECT_TRUE(called);
    EXPECT_EQ(received_value, 42);
}

TEST(EventEmitterTest, OnceSingleCall) {
    nathan::TestEmitter emitter;
    
    int call_count = 0;
    
    auto token = emitter.once<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) {
            call_count++;
        }
    );
    
    EXPECT_TRUE(token.is_valid());
    
    nathan::TestEvent event1{1};
    emitter.emit_test<nathan::TestEvent>("test_event", event1);
    EXPECT_EQ(call_count, 1);
    
    nathan::TestEvent event2{2};
    emitter.emit_test<nathan::TestEvent>("test_event", event2);
    EXPECT_EQ(call_count, 1); // Should not be called again
}

TEST(EventEmitterTest, OffDisconnect) {
    nathan::TestEmitter emitter;
    
    int call_count = 0;
    
    auto token = emitter.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) {
            call_count++;
        }
    );
    
    nathan::TestEvent event{0};
    emitter.emit_test<nathan::TestEvent>("test_event", event);
    EXPECT_EQ(call_count, 1);
    
    // Disconnect
    emitter.off(token);
    
    emitter.emit_test<nathan::TestEvent>("test_event", event);
    EXPECT_EQ(call_count, 1); // Should not have increased
}

TEST(EventEmitterTest, OffByEventType) {
    nathan::TestEmitter emitter;
    
    int call_count = 0;
    
    emitter.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    nathan::TestEvent event{0};
    emitter.emit_test<nathan::TestEvent>("test_event", event);
    EXPECT_EQ(call_count, 1);
    
    // Disconnect all for this event type
    emitter.off("test_event");
    
    emitter.emit_test<nathan::TestEvent>("test_event", event);
    EXPECT_EQ(call_count, 1);
}

TEST(EventEmitterTest, OffAll) {
    nathan::TestEmitter emitter;
    
    int call_count1 = 0;
    int call_count2 = 0;
    
    emitter.on<nathan::TestEvent>(
        "event1",
        [&](const nathan::TestEvent&) { call_count1++; }
    );
    
    emitter.on<nathan::AnotherEvent>(
        "event2",
        [&](const nathan::AnotherEvent&) { call_count2++; }
    );
    
    nathan::TestEvent event1{0};
    emitter.emit_test<nathan::TestEvent>("event1", event1);
    nathan::AnotherEvent event2{0.0f};
    emitter.emit_test<nathan::AnotherEvent>("event2", event2);
    
    EXPECT_EQ(call_count1, 1);
    EXPECT_EQ(call_count2, 1);
    
    // Disconnect all
    emitter.off_all();
    
    emitter.emit_test<nathan::TestEvent>("event1", event1);
    emitter.emit_test<nathan::AnotherEvent>("event2", event2);
    
    EXPECT_EQ(call_count1, 1);
    EXPECT_EQ(call_count2, 1);
}

TEST(EventEmitterTest, MultipleSubscriptionsSameEvent) {
    nathan::TestEmitter emitter;
    
    int call_count = 0;
    
    emitter.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    emitter.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    emitter.on<nathan::TestEvent>(
        "test_event",
        [&](const nathan::TestEvent&) { call_count++; }
    );
    
    nathan::TestEvent event{0};
    emitter.emit_test<nathan::TestEvent>("test_event", event);
    
    EXPECT_EQ(call_count, 3);
}

TEST(EventEmitterTest, DifferentEventTypes) {
    nathan::TestEmitter emitter;
    
    bool test_event_called = false;
    bool another_event_called = false;
    
    emitter.on<nathan::TestEvent>(
        "test",
        [&](const nathan::TestEvent&) { test_event_called = true; }
    );
    
    emitter.on<nathan::AnotherEvent>(
        "another",
        [&](const nathan::AnotherEvent&) { another_event_called = true; }
    );
    
    nathan::TestEvent test_event{0, ""};
    emitter.emit_test<nathan::TestEvent>("test", test_event);
    
    EXPECT_TRUE(test_event_called);
    EXPECT_FALSE(another_event_called);
    
    nathan::AnotherEvent another_event{0.0f};
    emitter.emit_test<nathan::AnotherEvent>("another", another_event);
    
    EXPECT_TRUE(another_event_called);
}

TEST(EventEmitterTest, InvalidTokenOff) {
    nathan::TestEmitter emitter;
    
    nathan::ConnectionToken invalid_token;
    
    // Should not crash
    emitter.off(invalid_token);
    
    SUCCEED();
}

TEST(EventEmitterTest, EmitToNonExistentEventType) {
    nathan::TestEmitter emitter;
    
    nathan::TestEvent event{0, ""};
    
    // Should not crash
    emitter.emit_test<nathan::TestEvent>("non_existent", event);
    
    SUCCEED();
}

// Unit tests for EventBus using Google Test

#include <gtest/gtest.h>
#include <string>

#include "gameplay_foundations/events/event_bus.hpp"

namespace nathan {

struct GlobalTestEvent {
    int value;
};

} // namespace nathan

TEST(EventBusTest, GlobalReturnsSingleton) {
    nathan::EventBus& bus1 = nathan::EventBus::global();
    nathan::EventBus& bus2 = nathan::EventBus::global();
    
    EXPECT_EQ(&bus1, &bus2);
}

TEST(EventBusTest, EmitGlobalAndOnGlobal) {
    nathan::EventBus& bus = nathan::EventBus::global();
    
    // Clear any existing subscriptions
    bus.off_all();
    
    bool called = false;
    int received_value = 0;
    
    auto token = bus.on_global<nathan::GlobalTestEvent>(
        "global_test",
        [&](const nathan::GlobalTestEvent& event) {
            called = true;
            received_value = event.value;
        }
    );
    
    EXPECT_TRUE(token.is_valid());
    
    nathan::GlobalTestEvent event{123};
    bus.emit_global<nathan::GlobalTestEvent>("global_test", event);
    
    EXPECT_TRUE(called);
    EXPECT_EQ(received_value, 123);
    
    // Clean up
    bus.off_all();
}

TEST(EventBusTest, GlobalIsAccessibleFromAnywhere) {
    nathan::EventBus& bus = nathan::EventBus::global();
    
    // Clear any existing subscriptions
    bus.off_all();
    
    bool called1 = false;
    
    bus.on_global<nathan::GlobalTestEvent>(
        "test_global",
        [&](const nathan::GlobalTestEvent&) { called1 = true; }
    );
    
    // Second access
    nathan::EventBus& bus2 = nathan::EventBus::global();
    nathan::GlobalTestEvent event{0};
    bus2.emit_global<nathan::GlobalTestEvent>("test_global", event);
    
    EXPECT_TRUE(called1);
    EXPECT_EQ(&bus, &bus2);
    
    // Clean up
    bus.off_all();
}

TEST(EventBusTest, MultipleGlobalSubscriptions) {
    nathan::EventBus& bus = nathan::EventBus::global();
    
    // Clear any existing subscriptions
    bus.off_all();
    
    int call_count = 0;
    
    bus.on_global<nathan::GlobalTestEvent>(
        "test_event",
        [&](const nathan::GlobalTestEvent&) { call_count++; }
    );
    
    bus.on_global<nathan::GlobalTestEvent>(
        "test_event",
        [&](const nathan::GlobalTestEvent&) { call_count++; }
    );
    
    nathan::GlobalTestEvent event{0};
    bus.emit_global<nathan::GlobalTestEvent>("test_event", event);
    
    EXPECT_EQ(call_count, 2);
    
    // Clean up
    bus.off_all();
}

TEST(EventBusTest, OffAllClearsAllGlobalSubscriptions) {
    nathan::EventBus& bus = nathan::EventBus::global();
    
    // Clear any existing subscriptions first
    bus.off_all();
    
    int call_count = 0;
    
    bus.on_global<nathan::GlobalTestEvent>(
        "test1",
        [&](const nathan::GlobalTestEvent&) { call_count++; }
    );
    
    bus.on_global<nathan::GlobalTestEvent>(
        "test2",
        [&](const nathan::GlobalTestEvent&) { call_count++; }
    );
    
    nathan::GlobalTestEvent event{0};
    bus.emit_global<nathan::GlobalTestEvent>("test1", event);
    bus.emit_global<nathan::GlobalTestEvent>("test2", event);
    
    EXPECT_EQ(call_count, 2);
    
    bus.off_all();
    
    bus.emit_global<nathan::GlobalTestEvent>("test1", event);
    bus.emit_global<nathan::GlobalTestEvent>("test2", event);
    
    EXPECT_EQ(call_count, 2); // No new calls
}

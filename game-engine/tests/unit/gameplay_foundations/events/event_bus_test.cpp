// Unit tests for EventBus using Google Test

#include <gtest/gtest.h>
#include <string>

#include "gameplay_foundations/events/event_bus.hpp"

namespace nathan {

struct GlobalTestEvent {
    int value;
};

} // namespace nathan

TEST(EventBusTest, CreateInstance) {
    nathan::EventBus bus1;
    nathan::EventBus bus2;
    
    // Different instances should be different objects
    EXPECT_NE(&bus1, &bus2);
}

TEST(EventBusTest, EmitGlobalAndOnGlobal) {
    nathan::EventBus bus;
    
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
}

TEST(EventBusTest, InstanceIsAccessibleAndWorks) {
    nathan::EventBus bus;
    
    bool called1 = false;
    
    bus.on_global<nathan::GlobalTestEvent>(
        "test_global",
        [&](const nathan::GlobalTestEvent&) { called1 = true; }
    );
    
    nathan::GlobalTestEvent event{0};
    bus.emit_global<nathan::GlobalTestEvent>("test_global", event);
    
    EXPECT_TRUE(called1);
}

TEST(EventBusTest, MultipleGlobalSubscriptions) {
    nathan::EventBus bus;
    
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
}

TEST(EventBusTest, OffAllClearsAllGlobalSubscriptions) {
    nathan::EventBus bus;
    
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

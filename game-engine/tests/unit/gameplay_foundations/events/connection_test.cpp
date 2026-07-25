// Unit tests for ConnectionToken using Google Test

#include <gtest/gtest.h>

#include "gameplay_foundations/events/connection.hpp"

TEST(ConnectionTokenTest, DefaultConstruction) {
    nathan::ConnectionToken token;
    
    EXPECT_FALSE(token.is_valid());
    EXPECT_EQ(token.get_id(), 0);
    EXPECT_EQ(token.get_emitter(), nullptr);
}

TEST(ConnectionTokenTest, ConstructionWithEmitter) {
    int emitter = 42;
    nathan::ConnectionToken token(static_cast<void*>(&emitter));
    
    EXPECT_TRUE(token.is_valid());
    EXPECT_NE(token.get_id(), 0);
    EXPECT_EQ(token.get_emitter(), static_cast<void*>(&emitter));
}

TEST(ConnectionTokenTest, EqualityOperators) {
    int emitter1 = 1;
    int emitter2 = 2;
    
    nathan::ConnectionToken token1(static_cast<void*>(&emitter1));
    nathan::ConnectionToken token2(static_cast<void*>(&emitter1));
    nathan::ConnectionToken token3(static_cast<void*>(&emitter2));
    nathan::ConnectionToken token4;
    
    EXPECT_EQ(token1, token1);
    EXPECT_NE(token1, token2); // Different IDs
    EXPECT_NE(token1, token3); // Different emitters
    EXPECT_NE(token1, token4); // Invalid token
}

TEST(ConnectionTokenTest, IDsAreUnique) {
    int emitter = 0;
    
    nathan::ConnectionToken token1(static_cast<void*>(&emitter));
    nathan::ConnectionToken token2(static_cast<void*>(&emitter));
    nathan::ConnectionToken token3(static_cast<void*>(&emitter));
    
    EXPECT_NE(token1.get_id(), token2.get_id());
    EXPECT_NE(token1.get_id(), token3.get_id());
    EXPECT_NE(token2.get_id(), token3.get_id());
}

TEST(ConnectionTokenTest, IsValidRequiresBothIdAndEmitter) {
    int emitter = 0;
    nathan::ConnectionToken valid_token(static_cast<void*>(&emitter));
    nathan::ConnectionToken invalid_token;
    
    EXPECT_TRUE(valid_token.is_valid());
    EXPECT_FALSE(invalid_token.is_valid());
}

TEST(ConnectionTokenTest, NullEmitterIsInvalid) {
    nathan::ConnectionToken token(static_cast<void*>(nullptr));
    
    // Token with null emitter is invalid even if ID != 0
    EXPECT_FALSE(token.is_valid());
}

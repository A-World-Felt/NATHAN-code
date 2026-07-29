// Unit tests for Config class using Google Test

#include <gtest/gtest.h>
#include <string>

#include "gameplay_foundations/resources/config.hpp"

TEST(ConfigTest, DefaultConstructor) {
    nathan::Config config;
    EXPECT_TRUE(config.get_path().empty());
    EXPECT_FALSE(config.is_dirty());
}

TEST(ConfigTest, PathAccess) {
    nathan::Config config;
    config.set_path("test.toml");
    EXPECT_EQ(config.get_path(), "test.toml");
}

TEST(ConfigTest, TypeAccessors) {
    nathan::Config config;
    
    // Test all setters mark dirty
    config.set_float("float_val", 3.14f);
    config.set_int("int_val", 42);
    config.set_bool("bool_val", true);
    config.set_string("string_val", "test");
    EXPECT_TRUE(config.is_dirty());
    
    // Test all getters
    EXPECT_EQ(config.get_float("float_val", 0.0f), 3.14f);
    EXPECT_EQ(config.get_int("int_val", 0), 42);
    EXPECT_EQ(config.get_bool("bool_val", false), true);
    EXPECT_EQ(config.get_string("string_val", ""), "test");
    
    // Test default values for missing keys
    EXPECT_EQ(config.get_float("missing", 1.5f), 1.5f);
    EXPECT_EQ(config.get_int("missing", 10), 10);
    EXPECT_EQ(config.get_bool("missing", true), true);
    EXPECT_EQ(config.get_string("missing", "default"), "default");
}

TEST(ConfigTest, SameValueDoesNotMarkDirty) {
    nathan::Config config;
    config.set_path("test_dirty.toml");
    
    config.set_float("test", 1.0f);
    EXPECT_TRUE(config.is_dirty());
    
    config.save();
    EXPECT_FALSE(config.is_dirty());
    
    config.set_float("test", 1.0f);
    EXPECT_FALSE(config.is_dirty());
    
    std::remove("test_dirty.toml");
}

TEST(ConfigTest, TypeConversion) {
    nathan::Config config;
    
    config.set_int("test", 42);
    EXPECT_EQ(config.get_float("test", 0.0f), 42.0f);
    
    config.set_float("test2", 3.14f);
    EXPECT_EQ(config.get_int("test2", 0), 3);
}

TEST(ConfigTest, LoadAndSave) {
    const char kPath[] = "test_save_load.toml";
    
    nathan::Config config;
    config.set_path(kPath);
    
    config.set_float("float_val", 3.14f);
    config.set_int("int_val", 42);
    config.set_bool("bool_val", true);
    config.set_string("string_val", "test");
    
    EXPECT_TRUE(config.save());
    EXPECT_FALSE(config.is_dirty());
    
    nathan::Config config2;
    config2.set_path(kPath);
    EXPECT_TRUE(config2.load());
    
    EXPECT_EQ(config2.get_float("float_val", 0.0f), 3.14f);
    EXPECT_EQ(config2.get_int("int_val", 0), 42);
    EXPECT_EQ(config2.get_bool("bool_val", false), true);
    EXPECT_EQ(config2.get_string("string_val", ""), "test");
    
    std::remove(kPath);
}

TEST(ConfigTest, LoadWithoutPathFails) {
    nathan::Config config;
    EXPECT_FALSE(config.load());
}

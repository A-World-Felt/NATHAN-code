// Unit tests for ConfigManager using Google Test

#include <gtest/gtest.h>
#include <string>

#include "gameplay_foundations/resources/config_manager.hpp"

namespace nathan {

} // namespace nathan

TEST(ConfigManagerTest, DefaultConstructorCreatesEmptyConfig) {
    nathan::ConfigManager config;
    EXPECT_EQ(config.get_config_path(), "config.json");
    EXPECT_FALSE(config.is_dirty());
}

TEST(ConfigManagerTest, ConstructorWithPath) {
    nathan::ConfigManager config("custom_config.json");
    EXPECT_EQ(config.get_config_path(), "custom_config.json");
}

TEST(ConfigManagerTest, SetAndGetConfigPath) {
    nathan::ConfigManager config;
    config.set_config_path("new_config.json");
    EXPECT_EQ(config.get_config_path(), "new_config.json");
}

TEST(ConfigManagerTest, GetSetFloat) {
    nathan::ConfigManager config;
    
    // Default value
    EXPECT_EQ(config.get_float("test_float"), 0.0f);
    EXPECT_EQ(config.get_float("test_float", 1.5f), 1.5f);
    
    // Set and get
    config.set_float("test_float", 3.14f);
    EXPECT_EQ(config.get_float("test_float"), 3.14f);
    EXPECT_TRUE(config.is_dirty());
}

TEST(ConfigManagerTest, GetSetInt) {
    nathan::ConfigManager config;
    
    // Default value
    EXPECT_EQ(config.get_int("test_int"), 0);
    EXPECT_EQ(config.get_int("test_int", 42), 42);
    
    // Set and get
    config.set_int("test_int", 123);
    EXPECT_EQ(config.get_int("test_int"), 123);
    EXPECT_TRUE(config.is_dirty());
}

TEST(ConfigManagerTest, GetSetBool) {
    nathan::ConfigManager config;
    
    // Default value
    EXPECT_EQ(config.get_bool("test_bool"), false);
    EXPECT_EQ(config.get_bool("test_bool", true), true);
    
    // Set and get
    config.set_bool("test_bool", true);
    EXPECT_EQ(config.get_bool("test_bool"), true);
    EXPECT_TRUE(config.is_dirty());
}

TEST(ConfigManagerTest, GetSetString) {
    nathan::ConfigManager config;
    
    // Default value
    EXPECT_EQ(config.get_string("test_string"), "");
    EXPECT_EQ(config.get_string("test_string", "default"), "default");
    
    // Set and get
    config.set_string("test_string", "hello");
    EXPECT_EQ(config.get_string("test_string"), "hello");
    EXPECT_TRUE(config.is_dirty());
}

TEST(ConfigManagerTest, SameValueDoesNotMarkDirty) {
    nathan::ConfigManager config;
    
    config.set_float("test", 1.0f);
    EXPECT_TRUE(config.is_dirty());
    
    // Reset dirty flag
    config.save_if_dirty();
    
    // Set to same value - should not mark dirty
    config.set_float("test", 1.0f);
    EXPECT_FALSE(config.is_dirty());
}

TEST(ConfigManagerTest, LoadEmptyFile) {
    nathan::ConfigManager config;
    
    // Try to load a non-existent file
    EXPECT_FALSE(config.load("nonexistent_config.json"));
}

TEST(ConfigManagerTest, SaveAndLoad) {
    nathan::ConfigManager config("test_config_save_load.json");
    
    // Set some values
    config.set_float("float_val", 3.14f);
    config.set_int("int_val", 42);
    config.set_bool("bool_val", true);
    config.set_string("string_val", "test");
    
    // Save
    EXPECT_TRUE(config.save());
    EXPECT_FALSE(config.is_dirty());
    
    // Create new config and load
    nathan::ConfigManager config2("test_config_save_load.json");
    EXPECT_TRUE(config2.load());
    
    EXPECT_EQ(config2.get_float("float_val"), 3.14f);
    EXPECT_EQ(config2.get_int("int_val"), 42);
    EXPECT_EQ(config2.get_bool("bool_val"), true);
    EXPECT_EQ(config2.get_string("string_val"), "test");
    
    // Cleanup
    std::remove("test_config_save_load.json");
}

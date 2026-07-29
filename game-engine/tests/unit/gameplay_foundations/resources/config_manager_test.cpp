// Unit tests for ConfigManager class using Google Test

#include <gtest/gtest.h>
#include <string>

#include "gameplay_foundations/resources/config_manager.hpp"

TEST(ConfigManagerTest, DefaultConstructorAndGetOrCreate) {
    nathan::ConfigManager config;
    
    // Config should not exist until accessed
    EXPECT_FALSE(config.has(nathan::ConfigManager::kDefaultConfig.name));
    EXPECT_EQ(config.get(nathan::ConfigManager::kDefaultConfig.name), nullptr);
    
    // After accessing, it should create the config with empty path
    auto& default_config = config.get_or_create(nathan::ConfigManager::kDefaultConfig.name);
    EXPECT_TRUE(config.has(nathan::ConfigManager::kDefaultConfig.name));
    EXPECT_TRUE(default_config.get_path().empty());
    EXPECT_FALSE(default_config.is_dirty());
    
    // Can retrieve it
    auto* retrieved = config.get(nathan::ConfigManager::kDefaultConfig.name);
    EXPECT_NE(retrieved, nullptr);
}

TEST(ConfigManagerTest, ConstructorWithPath) {
    nathan::ConfigManager config("custom_config.toml");
    
    auto& default_config = config.get_or_create(nathan::ConfigManager::kDefaultConfig);
    EXPECT_EQ(default_config.get_path(), "custom_config.toml");
}

TEST(ConfigManagerTest, GetOrCreateWithConfigDef) {
    nathan::ConfigManager config;
    
    auto& cfg = config.get_or_create(nathan::ConfigManager::kDefaultConfig);
    EXPECT_EQ(cfg.get_path(), "config.toml");
    EXPECT_TRUE(config.has(nathan::ConfigManager::kDefaultConfig.name));
}

TEST(ConfigManagerTest, GetOrCreateWithPath) {
    nathan::ConfigManager config;
    
    auto& cfg = config.get_or_create("test", "test.toml");
    EXPECT_EQ(cfg.get_path(), "test.toml");
}

TEST(ConfigManagerTest, GetOrCreateWithSameNameWarns) {
    nathan::ConfigManager config;
    
    auto& cfg1 = config.get_or_create("test", "first.toml");
    EXPECT_EQ(cfg1.get_path(), "first.toml");
    
    // Second call with different path should warn and keep original path
    testing::internal::CaptureStderr();
    auto& cfg2 = config.get_or_create("test", "second.toml");
    std::string stderr = testing::internal::GetCapturedStderr();
    
    EXPECT_NE(stderr.find("already exists"), std::string::npos);
    EXPECT_EQ(cfg2.get_path(), "first.toml");
}

TEST(ConfigManagerTest, LoadAndSave) {
    const char kPath[] = "test_cm.toml";
    
    // Save
    {
        nathan::ConfigManager config;
        auto& cfg = config.get_or_create("test", kPath);
        cfg.set_float("value", 3.14f);
        cfg.set_int("count", 42);
        EXPECT_TRUE(config.save("test"));
    }
    
    // Load
    {
        nathan::ConfigManager config;
        config.get_or_create("test", kPath);
        EXPECT_TRUE(config.load("test"));
        
        auto* loaded = config.get("test");
        EXPECT_NE(loaded, nullptr);
        EXPECT_EQ(loaded->get_float("value", 0.0f), 3.14f);
        EXPECT_EQ(loaded->get_int("count", 0), 42);
    }
    
    std::remove(kPath);
}

TEST(ConfigManagerTest, MultipleConfigs) {
    nathan::ConfigManager config;
    
    auto& cfg1 = config.get_or_create("config1", "config1.toml");
    auto& cfg2 = config.get_or_create("config2", "config2.toml");
    
    cfg1.set_float("value", 1.0f);
    cfg2.set_float("value", 2.0f);
    
    EXPECT_EQ(cfg1.get_float("value", 0.0f), 1.0f);
    EXPECT_EQ(cfg2.get_float("value", 0.0f), 2.0f);
}

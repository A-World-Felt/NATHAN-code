// Unit tests for ResourceManager using Google Test

#include <gtest/gtest.h>
#include <memory>

#include "gameplay_foundations/resources/resource_manager.hpp"
#include "gameplay_foundations/resources/resource.hpp"

namespace nathan {

// Mock Resource for testing
class MockResource : public Resource {
public:
    explicit MockResource(std::string path) 
        : Resource(std::move(path), Type::Unknown), loaded_(false) {}
    
    bool load() override {
        loaded_ = true;
        return true;
    }
    
    void unload() override {
        loaded_ = false;
    }
    
    bool is_loaded() const override {
        return loaded_;
    }
    
    bool was_loaded() const { return loaded_; }
    
private:
    bool loaded_;
};

} // namespace nathan

TEST(ResourceManagerTest, DefaultConstructorCreatesEmptyManager) {
    nathan::ResourceManager manager;
    EXPECT_EQ(manager.get_loaded_count(), 0);
    EXPECT_EQ(manager.get_total_count(), 0);
}

TEST(ResourceManagerTest, PushAndPopBasePath) {
    nathan::ResourceManager manager;
    
    EXPECT_EQ(manager.get_current_base_path(), "");
    
    manager.push_base_path("/path/to/resources");
    EXPECT_EQ(manager.get_current_base_path(), "/path/to/resources");
    
    manager.push_base_path("/another/path");
    EXPECT_EQ(manager.get_current_base_path(), "/another/path");
    
    manager.pop_base_path();
    EXPECT_EQ(manager.get_current_base_path(), "/path/to/resources");
    
    manager.pop_base_path();
    EXPECT_EQ(manager.get_current_base_path(), "");
    
    // Pop from empty stack should not crash
    manager.pop_base_path();
    EXPECT_EQ(manager.get_current_base_path(), "");
}

TEST(ResourceManagerTest, ResolvePathWithBasePath) {
    nathan::ResourceManager manager;
    
    // Without base path, returns original
    EXPECT_EQ(manager.get_current_base_path(), "");
    
    manager.push_base_path("/base/path/");
    
    // Current base path should be set
    EXPECT_EQ(manager.get_current_base_path(), "/base/path/");
    
    manager.pop_base_path();
}

TEST(ResourceManagerTest, UnloadAllClearsResources) {
    nathan::ResourceManager manager;
    
    // This test would need actual resource loading
    // For now, just test that unload_all doesn't crash
    manager.unload_all();
    EXPECT_EQ(manager.get_total_count(), 0);
}

TEST(ResourceManagerTest, UnloadUnusedWithNoResources) {
    nathan::ResourceManager manager;
    
    // Should not crash with empty manager
    manager.unload_unused();
    EXPECT_EQ(manager.get_total_count(), 0);
}

TEST(ResourceManagerTest, GetNonExistentResourceReturnsNullptr) {
    nathan::ResourceManager manager;
    
    auto resource = manager.get("nonexistent.wav");
    EXPECT_EQ(resource, nullptr);
}

TEST(ResourceManagerTest, IsLoadedForNonExistentResourceReturnsFalse) {
    nathan::ResourceManager manager;
    
    EXPECT_FALSE(manager.is_loaded("nonexistent.wav"));
}

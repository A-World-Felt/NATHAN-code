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

TEST(ResourceManagerTest, PushBasePathUpdatesCurrentPath) {
    nathan::ResourceManager manager;
    manager.push_base_path("/path/to/resources");
    EXPECT_EQ(manager.get_current_base_path(), "/path/to/resources");
}

TEST(ResourceManagerTest, PushMultipleBasePathsStacksCorrectly) {
    nathan::ResourceManager manager;
    manager.push_base_path("/path/one");
    manager.push_base_path("/path/two");
    EXPECT_EQ(manager.get_current_base_path(), "/path/two");
}

TEST(ResourceManagerTest, PopBasePathReturnsToPrevious) {
    nathan::ResourceManager manager;
    manager.push_base_path("/path/one");
    manager.push_base_path("/path/two");
    manager.pop_base_path();
    EXPECT_EQ(manager.get_current_base_path(), "/path/one");
}

TEST(ResourceManagerTest, PopBasePathFromEmptyStackDoesNothing) {
    nathan::ResourceManager manager;
    manager.pop_base_path();
    EXPECT_EQ(manager.get_current_base_path(), "");
}

TEST(ResourceManagerTest, PopBasePathFromSingleEntryClearsPath) {
    nathan::ResourceManager manager;
    manager.push_base_path("/path/one");
    manager.pop_base_path();
    EXPECT_EQ(manager.get_current_base_path(), "");
}

TEST(ResourceManagerTest, GetNonExistentResourceReturnsNullptr) {
    nathan::ResourceManager manager;
    auto resource = manager.get("nonexistent");
    EXPECT_EQ(resource, nullptr);
}

TEST(ResourceManagerTest, IsLoadedForNonExistentResourceReturnsFalse) {
    nathan::ResourceManager manager;
    EXPECT_FALSE(manager.is_loaded("nonexistent"));
}

TEST(ResourceManagerTest, UnloadAllOnEmptyManagerDoesNothing) {
    nathan::ResourceManager manager;
    manager.unload_all();
    EXPECT_EQ(manager.get_total_count(), 0);
}

TEST(ResourceManagerTest, UnloadUnusedOnEmptyManagerDoesNothing) {
    nathan::ResourceManager manager;
    manager.unload_unused();
    EXPECT_EQ(manager.get_total_count(), 0);
}

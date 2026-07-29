#ifndef GAME_ENGINE_RESOURCES_RESOURCE_MANAGER_H_
#define GAME_ENGINE_RESOURCES_RESOURCE_MANAGER_H_

#include "resource.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace nathan {

class Resource;

// Manages loading, caching, and sharing of game resources
// Resources are reference-counted via shared_ptr for automatic cleanup
class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Delete copy/move
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    // Path resolution for per-context loading
    // This allows nested contexts (like minigames) to have their own base paths
    void push_base_path(std::string_view path);
    void pop_base_path();
    const std::string& get_current_base_path() const;

    // Load a resource - returns shared handle (automatic reference counting)
    // If already loaded, returns existing instance
    std::shared_ptr<Resource> load(std::string_view path);

    // Load a resource as a specific type (with automatic casting)
    template <typename T>
    std::shared_ptr<T> load_as(std::string_view path) {
        return std::static_pointer_cast<T>(load(path));
    }

    // Get existing resource by path without loading
    std::shared_ptr<Resource> get(std::string_view path) const;

    // Get a resource as a specific type (with automatic casting)
    template <typename T>
    std::shared_ptr<T> get_as(std::string_view path) const {
        return std::static_pointer_cast<T>(get(path));
    }

    // Check if a resource is loaded
    bool is_loaded(std::string_view path) const;

    // Unload all unused resources (reference count = 0)
    void unload_unused();

    // Unload a specific resource
    void unload(std::string_view path);

    // Unload all resources
    void unload_all();

    // Get statistics
    size_t get_loaded_count() const;
    size_t get_total_count() const;

private:
    // Resolves a path using the base path stack
    std::string resolve(std::string_view path) const;

    std::vector<std::string> base_path_stack_;
    std::unordered_map<std::string, std::shared_ptr<Resource>> resources_;
};

} // namespace nathan

#endif // GAME_ENGINE_RESOURCES_RESOURCE_MANAGER_H_

#include "resource_manager.hpp"
#include "text_resource.hpp"

#include <filesystem>
#include <iostream>

namespace nathan {

namespace fs = std::filesystem;

ResourceManager::ResourceManager() = default;

ResourceManager::~ResourceManager() {
    unload_all();
}

void ResourceManager::push_base_path(std::string_view path) {
    if (!path.empty()) {
        base_path_stack_.push_back(std::string(path));
    }
}

void ResourceManager::pop_base_path() {
    if (!base_path_stack_.empty()) {
        base_path_stack_.pop_back();
    }
}

const std::string& ResourceManager::get_current_base_path() const {
    if (base_path_stack_.empty()) {
        static const std::string empty;
        return empty;
    }
    return base_path_stack_.back();
}

std::string ResourceManager::resolve(std::string_view path) const {
    if (base_path_stack_.empty()) {
        return std::string(path);
    }

    // If path is already absolute, return it
    if (fs::path(path).is_absolute()) {
        return std::string(path);
    }

    // Resolve relative to current base path
    auto current_base = base_path_stack_.back();
    fs::path full_path = fs::path(current_base) / path;

    // Normalize the path (remove redundant separators, etc.)
    return fs::weakly_canonical(full_path).string();
}

std::shared_ptr<Resource> ResourceManager::load(std::string_view path) {
    std::string resolved_path = resolve(path);

    // Check if already loaded
    auto it = resources_.find(resolved_path);
    if (it != resources_.end()) {
        return it->second;
    }

    // Create new resource based on file extension
    std::shared_ptr<Resource> resource;
    fs::path p(resolved_path);
    std::string ext = p.extension().string();

    // Convert to lowercase for comparison
    for (char& c : ext) {
        c = static_cast<char>(::tolower(static_cast<unsigned char>(c)));
    }

    if (ext == ".txt") {
        resource = std::make_shared<TextResource>(resolved_path);
        if (resource->load()) {
            resources_[resolved_path] = resource;
            std::cout << "[ResourceManager] Loaded: " << resolved_path << std::endl;
            return resource;
        }
    }

    // Unsupported resource type
    std::cerr << "[ResourceManager] Unsupported resource type: " << resolved_path << std::endl;
    return nullptr;
}

std::shared_ptr<Resource> ResourceManager::get(std::string_view path) const {
    std::string resolved_path = resolve(path);
    auto it = resources_.find(resolved_path);
    if (it != resources_.end()) {
        return it->second;
    }
    return nullptr;
}

bool ResourceManager::is_loaded(std::string_view path) const {
    std::string resolved_path = resolve(path);
    auto it = resources_.find(resolved_path);
    if (it != resources_.end()) {
        return it->second->is_loaded();
    }
    return false;
}

void ResourceManager::unload_unused() {
    for (auto it = resources_.begin(); it != resources_.end(); ) {
        // If only the manager holds a reference (use_count == 1),
        // then no one else is using it
        if (it->second.use_count() == 1) {
            std::cout << "[ResourceManager] Unloading unused: " << it->first << std::endl;
            it->second->unload();
            it = resources_.erase(it);
        } else {
            ++it;
        }
    }
}

void ResourceManager::unload(std::string_view path) {
    std::string resolved_path = resolve(path);
    auto it = resources_.find(resolved_path);
    if (it != resources_.end()) {
        it->second->unload();
        resources_.erase(it);
        std::cout << "[ResourceManager] Unloaded: " << resolved_path << std::endl;
    }
}

void ResourceManager::unload_all() {
    for (auto& pair : resources_) {
        pair.second->unload();
    }
    resources_.clear();
    std::cout << "[ResourceManager] Unloaded all resources" << std::endl;
}

size_t ResourceManager::get_loaded_count() const {
    size_t count = 0;
    for (const auto& pair : resources_) {
        if (pair.second->is_loaded()) {
            ++count;
        }
    }
    return count;
}

size_t ResourceManager::get_total_count() const {
    return resources_.size();
}

} // namespace nathan

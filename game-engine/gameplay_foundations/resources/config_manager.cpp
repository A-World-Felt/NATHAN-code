#include "config_manager.hpp"

#include <iostream>

namespace nathan {

ConfigManager::ConfigManager(std::string_view default_config_path) {
    auto& config = configs_[std::string(kDefaultConfig.name)];
    config.set_path(default_config_path);
}

Config& ConfigManager::get_or_create(std::string_view name, std::string_view path) {
    auto it = configs_.find(std::string(name));
    if (it != configs_.end()) {
        if (!path.empty() && it->second.get_path() != path) {
            std::cerr << "[ConfigManager] Warning: Config '" << name 
                      << "' already exists with path '" << it->second.get_path()
                      << "', ignoring new path '" << path 
                      << "'. Use a different name for a different config." << std::endl;
        }
        return it->second;
    }
    
    auto [new_it, _] = configs_.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(std::string(name)),
        std::forward_as_tuple(Config())
    );
    if (!path.empty()) {
        new_it->second.set_path(path);
    }
    return new_it->second;
}

Config& ConfigManager::get_or_create(const ConfigDef& config_def) {
    return get_or_create(config_def.name, config_def.path);
}

Config* ConfigManager::get(std::string_view name) {
    auto it = configs_.find(std::string(name));
    if (it != configs_.end()) {
        return &it->second;
    }
    return nullptr;
}

const Config* ConfigManager::get(std::string_view name) const {
    auto it = configs_.find(std::string(name));
    if (it != configs_.end()) {
        return &it->second;
    }
    return nullptr;
}

bool ConfigManager::has(std::string_view name) const {
    return configs_.find(std::string(name)) != configs_.end();
}

bool ConfigManager::load(std::string_view name) {
    Config* config = get(name);
    if (!config) {
        std::cerr << "[ConfigManager] Config '" << name << "' not found" << std::endl;
        return false;
    }
    return config->load();
}

bool ConfigManager::save(std::string_view name) {
    Config* config = get(name);
    if (!config) {
        std::cerr << "[ConfigManager] Config '" << name << "' not found" << std::endl;
        return false;
    }
    return config->save();
}

} // namespace nathan

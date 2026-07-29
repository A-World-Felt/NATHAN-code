#ifndef GAME_ENGINE_RESOURCES_CONFIG_MANAGER_H_
#define GAME_ENGINE_RESOURCES_CONFIG_MANAGER_H_

#include <string>
#include <string_view>
#include <unordered_map>

#include "config.hpp"

namespace nathan {

// Predefined config definition - links name and path together
struct ConfigDef {
    std::string_view name;
    std::string_view path;
};

// Manages multiple named Config instances
class ConfigManager {
public:
    ConfigManager() = default;
    explicit ConfigManager(std::string_view default_config_path);

    // Delete copy/move
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    // Default config definition
    static constexpr ConfigDef kDefaultConfig{"default", "config.toml"};

    // Access a named config (creates if doesn't exist with optional path)
    Config& get_or_create(std::string_view name, std::string_view path = "");
    
    // Access a named config using a predefined ConfigDef (ensures name+path stay together)
    Config& get_or_create(const ConfigDef& config_def);
    
    // Access a named config (returns nullptr if doesn't exist)
    Config* get(std::string_view name);
    const Config* get(std::string_view name) const;
    
    // Check if a named config exists
    bool has(std::string_view name) const;

    // Load a named config from file (uses stored path)
    bool load(std::string_view name);

    // Save a named config to file (uses stored path)
    bool save(std::string_view name);

private:
    std::unordered_map<std::string, Config> configs_;
};

} // namespace nathan

#endif // GAME_ENGINE_RESOURCES_CONFIG_MANAGER_H_

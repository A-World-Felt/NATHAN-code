#ifndef GAME_ENGINE_RESOURCES_CONFIG_MANAGER_H_
#define GAME_ENGINE_RESOURCES_CONFIG_MANAGER_H_

#include <string>
#include <string_view>

#include "nlohmann/json.hpp"

namespace nathan {

// Manages persistent configuration settings stored in JSON files
class ConfigManager {
public:

    ConfigManager();
    explicit ConfigManager(std::string_view config_path);

    // Delete copy/move
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    // Path management
    void set_config_path(std::string_view path);
    std::string_view get_config_path() const;

    // Load from file
    bool load();
    bool load(std::string_view path);

    // Save to file
    bool save();
    bool save(std::string_view path);

    // Type-safe getters
    float get_float(std::string_view key, float default_value = 0.0f) const;
    int get_int(std::string_view key, int default_value = 0) const;
    bool get_bool(std::string_view key, bool default_value = false) const;
    std::string get_string(std::string_view key, std::string_view default_value = "") const;

    // Type-safe setters
    void set_float(std::string_view key, float value);
    void set_int(std::string_view key, int value);
    void set_bool(std::string_view key, bool value);
    void set_string(std::string_view key, std::string_view value);

    // Check if config has unsaved changes
    bool is_dirty() const;

    // Force save if dirty
    void save_if_dirty();

private:
    std::string config_path_ = "config.json";
    mutable bool dirty_ = false;
    nlohmann::json data_;
};

} // namespace nathan

#endif // GAME_ENGINE_RESOURCES_CONFIG_MANAGER_H_

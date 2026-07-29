#ifndef GAME_ENGINE_RESOURCES_CONFIG_H_
#define GAME_ENGINE_RESOURCES_CONFIG_H_

#include <string>
#include <string_view>

#include "toml.hpp"

namespace nathan {

class ConfigManager;

// Represents a single configuration set loaded from a TOML file
class Config {
public:
    Config() = default;

    // Load from file (uses stored path)
    bool load();

    // Save to file (uses stored path)
    bool save();

    // Path access
    std::string_view get_path() const { return path_; }
    void set_path(std::string_view path) { path_ = std::string(path); }

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

    // Dirty state
    bool is_dirty() const { return dirty_; }
    void save_if_dirty();

    // Direct access to underlying data (for advanced use)
    const toml::table& get_data() const { return data_; }
    toml::table& get_data() { return data_; }

private:
    std::string path_;
    bool dirty_ = false;
    toml::table data_;
};

} // namespace nathan

#endif // GAME_ENGINE_RESOURCES_CONFIG_H_

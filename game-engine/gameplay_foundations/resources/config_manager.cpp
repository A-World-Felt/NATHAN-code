#include "config_manager.hpp"

#include <fstream>
#include <iostream>

namespace nathan {

ConfigManager::ConfigManager() = default;

ConfigManager::ConfigManager(std::string_view config_path)
    : config_path_(config_path) {}

void ConfigManager::set_config_path(std::string_view path) {
    config_path_ = path;
    dirty_ = true;
}

std::string_view ConfigManager::get_config_path() const {
    return config_path_;
}

bool ConfigManager::load() {
    return load(config_path_);
}

bool ConfigManager::load(std::string_view path) {
    std::ifstream config_file{std::string(path)};
    if (!config_file.is_open()) {
        std::cerr << "[ConfigManager] Failed to open config file: " << path << std::endl;
        return false;
    }
    
    try {
        config_file >> data_;
        dirty_ = false;
        config_path_ = path;
        std::cout << "[ConfigManager] Loaded " << data_.size() << " settings from " << path << std::endl;
        return true;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "[ConfigManager] JSON parse error: " << e.what() << std::endl;
        return false;
    }
}

bool ConfigManager::save() {
    return save(config_path_);
}

bool ConfigManager::save(std::string_view path) {
    std::ofstream config_file{std::string(path)};
    if (!config_file.is_open()) {
        std::cerr << "[ConfigManager] Failed to open config file for writing: " << path << std::endl;
        return false;
    }
    
    try {
        config_file << data_.dump(2);
        dirty_ = false;
        std::cout << "[ConfigManager] Saved " << data_.size() << " settings to " << path << std::endl;
        return true;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "[ConfigManager] JSON serialization error: " << e.what() << std::endl;
        return false;
    }
}

float ConfigManager::get_float(std::string_view key, float default_value) const {
    if (!data_.contains(std::string(key))) return default_value;
    try { return data_.at(std::string(key)).get<float>(); }
    catch (...) { return default_value; }
}

int ConfigManager::get_int(std::string_view key, int default_value) const {
    if (!data_.contains(std::string(key))) return default_value;
    try { return data_.at(std::string(key)).get<int>(); }
    catch (...) { return default_value; }
}

bool ConfigManager::get_bool(std::string_view key, bool default_value) const {
    if (!data_.contains(std::string(key))) return default_value;
    try { return data_.at(std::string(key)).get<bool>(); }
    catch (...) { return default_value; }
}

std::string ConfigManager::get_string(std::string_view key, std::string_view default_value) const {
    if (!data_.contains(std::string(key))) return std::string(default_value);
    try { return data_.at(std::string(key)).get<std::string>(); }
    catch (...) { return std::string(default_value); }
}

void ConfigManager::set_float(std::string_view key, float value) {
    std::string key_str(key);
    try {
        if (!data_.contains(key_str) || data_.at(key_str).get<float>() != value) {
            data_[key_str] = value;
            dirty_ = true;
        }
    } catch (...) {
        data_[key_str] = value;
        dirty_ = true;
    }
}

void ConfigManager::set_int(std::string_view key, int value) {
    std::string key_str(key);
    try {
        if (!data_.contains(key_str) || data_.at(key_str).get<int>() != value) {
            data_[key_str] = value;
            dirty_ = true;
        }
    } catch (...) {
        data_[key_str] = value;
        dirty_ = true;
    }
}

void ConfigManager::set_bool(std::string_view key, bool value) {
    std::string key_str(key);
    try {
        if (!data_.contains(key_str) || data_.at(key_str).get<bool>() != value) {
            data_[key_str] = value;
            dirty_ = true;
        }
    } catch (...) {
        data_[key_str] = value;
        dirty_ = true;
    }
}

void ConfigManager::set_string(std::string_view key, std::string_view value) {
    std::string key_str(key);
    try {
        if (!data_.contains(key_str) || data_.at(key_str).get<std::string>() != std::string(value)) {
            data_[key_str] = std::string(value);
            dirty_ = true;
        }
    } catch (...) {
        data_[key_str] = std::string(value);
        dirty_ = true;
    }
}

bool ConfigManager::is_dirty() const {
    return dirty_;
}

void ConfigManager::save_if_dirty() {
    if (dirty_) save();
}

} // namespace nathan

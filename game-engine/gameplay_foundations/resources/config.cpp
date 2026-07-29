#include "config.hpp"

#include <fstream>
#include <iostream>

namespace nathan {

bool Config::load() {
    if (path_.empty()) {
        std::cerr << "[Config] Cannot load: no path set" << std::endl;
        return false;
    }
    
    try {
        data_ = toml::parse_file(path_);
        dirty_ = false;
        std::cout << "[Config] Loaded config from " << path_ << std::endl;
        return true;
    } catch (const toml::parse_error& e) {
        std::cerr << "[Config] TOML parse error loading " << path_ << ": " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "[Config] Failed to load config file: " << path_ << " - " << e.what() << std::endl;
        return false;
    }
}

bool Config::save() {
    if (path_.empty()) {
        std::cerr << "[Config] Cannot save: no path set" << std::endl;
        return false;
    }
    
    std::ofstream config_file{path_};
    if (!config_file.is_open()) {
        std::cerr << "[Config] Failed to open config file for writing: " << path_ << std::endl;
        return false;
    }
    
    try {
        config_file << data_;
        dirty_ = false;
        std::cout << "[Config] Saved config to " << path_ << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[Config] TOML serialization error: " << e.what() << std::endl;
        return false;
    }
}

float Config::get_float(std::string_view key, float default_value) const {
    if (auto val = data_[std::string(key)].value<double>()) {
        return static_cast<float>(*val);
    }
    if (auto val = data_[std::string(key)].value<int64_t>()) {
        return static_cast<float>(*val);
    }
    return default_value;
}

int Config::get_int(std::string_view key, int default_value) const {
    if (auto val = data_[std::string(key)].value<int64_t>()) {
        return static_cast<int>(*val);
    }
    if (auto val = data_[std::string(key)].value<double>()) {
        return static_cast<int>(*val);
    }
    return default_value;
}

bool Config::get_bool(std::string_view key, bool default_value) const {
    if (auto val = data_[std::string(key)].value<bool>()) {
        return *val;
    }
    return default_value;
}

std::string Config::get_string(std::string_view key, std::string_view default_value) const {
    if (auto val = data_[std::string(key)].value<std::string>()) {
        return *val;
    }
    return std::string(default_value);
}

void Config::set_float(std::string_view key, float value) {
    std::string key_str(key);
    double current = data_[key_str].value_or(static_cast<double>(0.0));
    if (!data_.contains(key_str) || current != static_cast<double>(value)) {
        data_.insert_or_assign(key_str, static_cast<double>(value));
        dirty_ = true;
    }
}

void Config::set_int(std::string_view key, int value) {
    std::string key_str(key);
    int64_t current = data_[key_str].value_or(static_cast<int64_t>(0));
    if (!data_.contains(key_str) || current != static_cast<int64_t>(value)) {
        data_.insert_or_assign(key_str, static_cast<int64_t>(value));
        dirty_ = true;
    }
}

void Config::set_bool(std::string_view key, bool value) {
    std::string key_str(key);
    bool current = data_[key_str].value_or(false);
    if (!data_.contains(key_str) || current != value) {
        data_.insert_or_assign(key_str, value);
        dirty_ = true;
    }
}

void Config::set_string(std::string_view key, std::string_view value) {
    std::string key_str(key);
    std::string value_str(value);
    std::string current = data_[key_str].value_or("");
    if (!data_.contains(key_str) || current != value_str) {
        data_.insert_or_assign(key_str, value_str);
        dirty_ = true;
    }
}

void Config::save_if_dirty() {
    if (dirty_) {
        save();
    }
}

} // namespace nathan

#include "text_resource.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace nathan {

TextResource::TextResource(std::string path)
    : Resource(std::move(path), Type::Text) {}

bool TextResource::load() {
    std::string file_path(get_path());
    std::ifstream input_file(file_path);
    if (!input_file.is_open()) {
        std::cerr << "[TextResource] Failed to open file: " << file_path << std::endl;
        loaded_ = false;
        return false;
    }

    std::stringstream buffer;
    buffer << input_file.rdbuf();
    content_ = buffer.str();
    loaded_ = true;

    std::cout << "[TextResource] Loaded: " << file_path << " (" << content_.size() << " bytes)" << std::endl;
    return true;
}

void TextResource::unload() {
    content_.clear();
    loaded_ = false;
    std::cout << "[TextResource] Unloaded: " << get_path() << std::endl;
}

bool TextResource::is_loaded() const {
    return loaded_;
}

} // namespace nathan

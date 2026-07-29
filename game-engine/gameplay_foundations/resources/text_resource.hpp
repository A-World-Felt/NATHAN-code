#ifndef GAME_ENGINE_RESOURCES_TEXT_RESOURCE_H_
#define GAME_ENGINE_RESOURCES_TEXT_RESOURCE_H_

#include <string>
#include <string_view>

#include "resource.hpp"

namespace nathan {

// Resource for loading plain text files
class TextResource : public Resource {
public:
    explicit TextResource(std::string path);
    ~TextResource() override = default;

    bool load() override;
    void unload() override;
    bool is_loaded() const override;

    // Access the loaded text content
    std::string_view get_content() const { return content_; }

private:
    std::string content_;
    bool loaded_ = false;
};

} // namespace nathan

#endif // GAME_ENGINE_RESOURCES_TEXT_RESOURCE_H_

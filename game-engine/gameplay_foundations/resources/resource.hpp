#ifndef GAME_ENGINE_RESOURCES_RESOURCE_H_
#define GAME_ENGINE_RESOURCES_RESOURCE_H_

#include <string>
#include <string_view>

namespace nathan {

// Base class for all resource types
// Uses reference counting via shared_ptr for automatic memory management
class Resource {
public:
    enum class Type {
        Unknown,
        // Future types can be added here (Audio, Texture, Font, etc.)
    };

    virtual ~Resource() = default;

    // Accessors
    std::string_view get_path() const { return path_; }
    Type get_type() const { return type_; }

    // Virtual load/unload - overridden by derived classes
    virtual bool load() = 0;
    virtual void unload() = 0;

    // Check if resource is currently loaded in memory
    virtual bool is_loaded() const = 0;

protected:
    explicit Resource(std::string path, Type type)
        : path_(std::move(path)), type_(type) {}

private:
    std::string path_;
    Type type_;
};

} // namespace nathan

#endif // GAME_ENGINE_RESOURCES_RESOURCE_H_

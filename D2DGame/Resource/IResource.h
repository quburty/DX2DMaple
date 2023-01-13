#pragma once

enum class ResourceType : uint
{
    Unknown,
    Animation,
    Audio,
    Texture2D,
};

class IResource
{
public:
    template <typename T>
    static constexpr ResourceType DeduceResourceType();

public:
    IResource(class Context* const context, const ResourceType& type);
    virtual ~IResource() = default;

    virtual bool SaveToFile(const std::string& path) = 0;
    virtual bool LoadFromFile(const std::string& path) = 0;

    auto GetResourceType()          const { return resource_type; }
    auto GetResourceName()          const { return resource_name; }
    auto GetResourceDirectory()     const { return resource_directory; }
    auto GetResourcePath_Native()   const { return resource_path_native; }
    auto GetResourcePath_Foreign()  const { return resource_path_foreign; }
    auto HasResourcePath_Native()   const { return !resource_path_native.empty(); }

    void SetResourcePath(const std::string& path);

protected:
    class Context* context              = nullptr;
    ResourceType resource_type          = ResourceType::Unknown;
    std::string resource_name           = "";
    std::string resource_directory      = "";
    std::string resource_path_native    = "";
    std::string resource_path_foreign   = "";
};
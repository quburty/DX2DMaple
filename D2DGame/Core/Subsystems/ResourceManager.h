#pragma once
#include "ISubsystem.h"
#include "Resource/IResource.h"

enum class AssetType : uint
{
    Shader,
    Texture,
    Audio,
    Icon,
    Script,
    Animation,
};

class ResourceManager final : public ISubsystem
{
public:
    ResourceManager(class Context* const context);
    ~ResourceManager() = default;

    bool Initialize() override;

    /*
        Importer
    */
    auto GetTextureImporter() const -> const std::shared_ptr<class TextureImporter>& { return texture_importer; }

    /*
        Resource
    */
    template <typename T>
    auto Load(const std::string& path) -> std::shared_ptr<T>;

    template <typename T>
    auto GetResourceFromName(const std::string& name) -> std::shared_ptr<T>;

    template <typename T>
    auto GetResourceFromPath(const std::string& path) -> std::shared_ptr<T>;

    template <typename T>
    void RegisterResource(const std::shared_ptr<T>& resource);

    bool HasResource(const std::string& resource_name, const ResourceType& resource_type);

    /*
        Directories
    */
    auto GetAssetDirectory() const -> const std::string { return "../_Assets/"; }
    auto GetAssetDirectory(const AssetType& type) -> const std::string;

private:
    void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
    using resource_group_t = std::vector<std::shared_ptr<IResource>>;
    std::map<ResourceType, resource_group_t> resource_groups;
    std::mutex resource_mutex;

    std::map<AssetType, std::string> asset_directories;

    std::shared_ptr<class TextureImporter> texture_importer;
};

template<typename T>
inline auto ResourceManager::Load(const std::string & path) -> std::shared_ptr<T>
{
    static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

    if (!FileSystem::IsExists(path))
    {
        assert(false);
        return nullptr;
    }

    auto resource_name = FileSystem::GetIntactFileNameFromPath(path);

    if (HasResource(resource_name, IResource::DeduceResourceType<T>()))
        return GetResourceFromName<T>(resource_name);

    auto resource = std::make_shared<T>(context);
    resource->SetResourcePath(path);

    if (!resource->LoadFromFile(path))
    {
        assert(false);
        return nullptr;
    }

    RegisterResource<T>(resource);

    return resource;
}

template<typename T>
inline auto ResourceManager::GetResourceFromName(const std::string & name) -> std::shared_ptr<T>
{
    static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

    for (const auto& resource : resource_groups[IResource::DeduceResourceType<T>()])
    {
        if (resource->GetResourceName() == name)
            return std::static_pointer_cast<T>(resource);
    }
    return nullptr;
}

template<typename T>
inline auto ResourceManager::GetResourceFromPath(const std::string & path) -> std::shared_ptr<T>
{
    static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

    for (const auto& resource : resource_groups[IResource::DeduceResourceType<T>()])
    {
        if (resource->GetResourcePath_Native() == path)
            return std::static_pointer_cast<T>(resource);
    }
    return nullptr;
}

template<typename T>
inline void ResourceManager::RegisterResource(const std::shared_ptr<T>& resource)
{
    static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

    if (!resource)
    {
        assert(false);
        return;
    }

    std::lock_guard<std::mutex> guard(resource_mutex);

    resource_groups[resource->GetResourceType()].emplace_back(resource);
}

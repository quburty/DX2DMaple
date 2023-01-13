#include "stdafx.h"
#include "ResourceManager.h"
#include "Resource/Importer/TextureImporter.h"

ResourceManager::ResourceManager(Context * const context)
    : ISubsystem(context)
{
    auto root_directory = GetAssetDirectory();

    RegisterDirectory(AssetType::Animation, root_directory + "Animations/");
    RegisterDirectory(AssetType::Audio,     root_directory + "Audios/");
    RegisterDirectory(AssetType::Texture,   root_directory + "Textures/");
    RegisterDirectory(AssetType::Shader,    root_directory + "Shaders/");
    RegisterDirectory(AssetType::Script,    root_directory + "Scripts/");
    RegisterDirectory(AssetType::Icon,      root_directory + "Icons/");
}

bool ResourceManager::Initialize()
{
    texture_importer = std::make_shared<TextureImporter>(context);

    return true;
}

bool ResourceManager::HasResource(const std::string & resource_name, const ResourceType & resource_type)
{
    if (resource_name == "" || resource_type == ResourceType::Unknown)
    {
        assert(false);
        return false;
    }

    for (const auto& resource : resource_groups[resource_type])
    {
        if (resource->GetResourceName() == resource_name)
            return true;
    }
    return false;
}

auto ResourceManager::GetAssetDirectory(const AssetType & type) -> const std::string
{
    return asset_directories[type];
}

void ResourceManager::RegisterDirectory(const AssetType & type, const std::string & directory)
{
    asset_directories[type] = directory;
}

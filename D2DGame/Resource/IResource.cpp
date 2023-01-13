#include "stdafx.h"
#include "IResource.h"

IResource::IResource(Context * const context, const ResourceType & type)
    : context(context)
    , resource_type(type)
{
}

void IResource::SetResourcePath(const std::string & path)
{
    auto relative_path  = FileSystem::GetRelativeFromPath(path);
    bool is_native_file = FileSystem::IsNativeFile(path);

    if (is_native_file)
        resource_path_native = relative_path;
    else
    {
        if (!FileSystem::IsExists(relative_path))
        {
            LOG_ERROR_F("\"%s\" is not a valid file path", path.c_str());
            return;
        }

        resource_path_foreign   = relative_path;
        resource_path_native    = FileSystem::GetNativePath(relative_path);
    }

    resource_name       = FileSystem::GetIntactFileNameFromPath(relative_path);
    resource_directory  = FileSystem::GetDirectoryFromPath(relative_path);
}

template <typename T>
constexpr ResourceType IResource::DeduceResourceType()
{
    return ResourceType::Unknown;
}

//explicit template instantiation
#define REGISTER_RESOURCE_TYPE(T, enum_type) template <> ResourceType IResource::DeduceResourceType<T>() { return enum_type; }
REGISTER_RESOURCE_TYPE(Animation, ResourceType::Animation)
REGISTER_RESOURCE_TYPE(AudioClip, ResourceType::Audio)
REGISTER_RESOURCE_TYPE(Texture2D, ResourceType::Texture2D)

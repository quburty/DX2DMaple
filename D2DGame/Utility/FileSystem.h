#pragma once

class FileSystem final
{
public:
    static auto Create_Directory(const std::string& path) -> const bool;
    static auto Delete_Directory(const std::string& path) -> const bool;
    static auto Delete_File(const std::string& path) -> const bool;
    static auto Copy_File(const std::string& src, const std::string& dst) -> const bool;
    static auto IsDirectory(const std::string& path) -> const bool;
    static auto IsFile(const std::string& path) -> const bool;
    static auto IsExists(const std::string& path) -> const bool;

    static auto GetNativePath(const std::string& path) -> const std::string;
    static auto IsNativeFile(const std::string& path) -> const bool;
    static auto IsSupportedTextureFile(const std::string& path) -> const bool;
    static auto IsSupportedAudioFile(const std::string& path) -> const bool;
    static auto IsSupportedScriptFile(const std::string& path) -> const bool;

    static auto GetFileNameFromPath(const std::string& path) -> const std::string;
    static auto GetIntactFileNameFromPath(const std::string& path) -> const std::string;
    static auto GetDirectoryFromPath(const std::string& path) -> const std::string;
    static auto GetExtensionFromPath(const std::string& path) -> const std::string;
    static auto GetPathWithoutExtension(const std::string& path) -> const std::string;
    static auto GetRelativeFromPath(const std::string& absolute_path) -> const std::string;
    static auto GetParentDirectory(const std::string& path) -> const std::string;
    static auto GetWorkingDirectory() -> const std::string;
    static auto GetDirectoriesInDirectory(const std::string& directory) -> const std::vector<std::string>;
    static auto GetFilesInDirectory(const std::string& directory) -> const std::vector<std::string>;

    static auto ToUppercase(const std::string& lower) -> const std::string;
    static auto ToLowercase(const std::string& upper) -> const std::string;
    static auto ToString(const std::wstring& wstr) -> const std::string;
    static auto ToWString(const std::string& str) -> const std::wstring;

    static void ReplaceAll(std::string& in_out_str, const std::string& from, const std::string& to);
    static auto IsEmptyOrWhitespace(const std::string& str) -> const bool;
    static auto IsAlphanumeric(const std::string& str) -> const bool;
    static auto GetStringBeforeExpression(const std::string& str, const std::string& exp) -> const std::string;
    static auto GetStringAfterExpression(const std::string& str, const std::string& exp) -> const std::string;
    static auto GetStringBetweenExpression(const std::string& str, const std::string& exp_a, const std::string& exp_b) -> const std::string;

public:
    static const char* NOT_ASSIGNED_STR;
    static const char* EXTENSION_SCENE;
    static const char* EXTENSION_MATERIAL;
    static const char* EXTENSION_MESH;
    static const char* EXTENSION_TEXTURE;
    static const char* EXTENSION_AUDIO;

    static const std::vector<std::string> supported_texture_formats;
    static const std::vector<std::string> supported_audio_formats;
    static const std::vector<std::string> supported_script_formats;
};
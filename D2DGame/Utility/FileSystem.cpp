#include "stdafx.h"
#include "FileSystem.h"
#include <filesystem>
#include <regex>
#include <cctype>
#include <shellapi.h>
using namespace std::filesystem;

const char* FileSystem::NOT_ASSIGNED_STR    = "N/A";
const char* FileSystem::EXTENSION_SCENE     = ".scene";
const char* FileSystem::EXTENSION_MATERIAL  = ".material";
const char* FileSystem::EXTENSION_MESH      = ".mesh";
const char* FileSystem::EXTENSION_TEXTURE   = ".texture";
const char* FileSystem::EXTENSION_AUDIO     = ".audio";

const std::vector<std::string> FileSystem::supported_texture_formats
{
    ".jpg",
    ".png",
    ".bmp",
    ".tga",
    ".dds",
    ".exr",
    ".raw",
    ".gif",
    ".hdr",
    ".ico",
    ".iff",
    ".jng",
    ".jpeg",
    ".koala",
    ".kodak",
    ".mng",
    ".pcx",
    ".pbm",
    ".pgm",
    ".ppm",
    ".pfm",
    ".pict",
    ".psd",
    ".raw",
    ".sgi",
    ".targa",
    ".tiff",
    ".tif",
    ".wbmp",
    ".webp",
    ".xbm",
    ".xpm"
};
const std::vector<std::string> FileSystem::supported_audio_formats
{
    ".aiff",
    ".asf",
    ".asx",
    ".dls",
    ".flac",
    ".fsb",
    ".it",
    ".m3u",
    ".midi",
    ".mod",
    ".mp2",
    ".mp3",
    ".ogg",
    ".pls",
    ".s3m",
    ".vag", // PS2/PSP
    ".wav",
    ".wax",
    ".wma",
    ".xm",
    ".xma" // XBOX 360
};
const std::vector<std::string> FileSystem::supported_script_formats
{
    ".as",
};

auto FileSystem::Create_Directory(const std::string & path) -> const bool
{
    try
    {
        return create_directories(path);
    }
    catch (filesystem_error& error)
    {
		LOG_ERROR_F("Error: %s at file %s", error.what(), path);
        return false;
    }
}

auto FileSystem::Delete_Directory(const std::string & path) -> const bool
{
    try
    {
        return remove_all(path) > 0ull;
    }
    catch (filesystem_error& error)
    {
		LOG_ERROR_F("Error: %s at file %s", error.what(), path);
        return false;
    }
}

auto FileSystem::Delete_File(const std::string & path) -> const bool
{
    if (is_directory(path))
        return false;

    try
    {
        return remove(path);
    }
    catch (filesystem_error& error)
    {
		LOG_ERROR_F("Error: %s at file %s", error.what(), path);
        return false;
    }
}

auto FileSystem::Copy_File(const std::string & src, const std::string & dst) -> const bool
{
    if (src == dst)
        return true;

    if (!IsExists(GetDirectoryFromPath(dst)))
        Create_Directory(GetDirectoryFromPath(dst));

    try
    {
        return copy_file(src, dst, copy_options::overwrite_existing);
    }
    catch (filesystem_error& error)
    {
		LOG_ERROR("Could not copy \"" + src + "\", " + std::string(error.what()));
        return false;
    }
}

auto FileSystem::IsDirectory(const std::string & path) -> const bool
{
    try
    {
        return exists(path) && is_directory(path);
    }
    catch (filesystem_error& error)
    {
		LOG_ERROR_F("Error: %s at file %s", error.what(), path);
        return false;
    }
}

auto FileSystem::IsFile(const std::string & path) -> const bool
{
    try
    {
        return exists(path) && is_regular_file(path);
    }
    catch (filesystem_error& error)
    {
		LOG_ERROR_F("Error: %s at file %s", error.what(), path);
        return false;
    }
}

auto FileSystem::IsExists(const std::string & path) -> const bool
{
    try
    {
        return exists(path);
    }
    catch (filesystem_error& error)
    {
		LOG_ERROR_F("Error: %s at file %s", error.what(), path);
        return false;
    }
}

auto FileSystem::GetNativePath(const std::string & path) -> const std::string
{
    auto file_path_without_extension = GetPathWithoutExtension(path);

    if (IsSupportedTextureFile(path))   return file_path_without_extension + EXTENSION_TEXTURE;
    if (IsSupportedAudioFile(path))     return file_path_without_extension + EXTENSION_AUDIO;

	LOG_WARNING("Failed to native file path");
    return path;
}

auto FileSystem::IsNativeFile(const std::string & path) -> const bool
{
    auto extension = GetExtensionFromPath(path);
    return
        extension == EXTENSION_SCENE    ||
        extension == EXTENSION_MATERIAL ||
        extension == EXTENSION_MESH     ||
        extension == EXTENSION_AUDIO    ||
        extension == EXTENSION_TEXTURE;
}

auto FileSystem::IsSupportedTextureFile(const std::string & path) -> const bool
{
    auto extension = GetExtensionFromPath(path);
    for (const auto& format : supported_texture_formats)
    {
        if (extension == format || extension == ToUppercase(format))
            return true;
    }
    return false;
}

auto FileSystem::IsSupportedAudioFile(const std::string & path) -> const bool
{
    auto extension = GetExtensionFromPath(path);
    for (const auto& format : supported_audio_formats)
    {
        if (extension == format || extension == ToUppercase(format))
            return true;
    }
    return false;
}

auto FileSystem::IsSupportedScriptFile(const std::string & path) -> const bool
{
    auto extension = GetExtensionFromPath(path);
    for (const auto& format : supported_script_formats)
    {
        if (extension == format || extension == ToUppercase(format))
            return true;
    }
    return false;
}

//find()                : 주어진 문자열이 존재하는 위치 -> 정방향
//rfind()               : 주어진 문자열이 존재하는 위치 -> 역방향
//find_first_of()       : 주어진 문자중에 하나라도 걸리는 첫번째 위치
//find_last_of()        : 주어진 문자중에 하나라도 걸리는 마지막 위치
//find_first_not_of()   : 주어진 문자가 아닌 문자가 걸리는 첫번째 위치
//find_last_not_of()    : 주어진 문자가 아닌 문자가 걸리는 마지막 위치

auto FileSystem::GetFileNameFromPath(const std::string & path) -> const std::string
{
    auto last_index = path.find_last_of("\\/");
    auto file_name  = path.substr(last_index + 1, path.length());

    return file_name;
}

auto FileSystem::GetIntactFileNameFromPath(const std::string & path) -> const std::string
{
    auto file_name          = GetFileNameFromPath(path);
    auto last_index         = file_name.find_last_of('.');
    auto intact_file_name   = file_name.substr(0, last_index);

    return intact_file_name;
}

auto FileSystem::GetDirectoryFromPath(const std::string & path) -> const std::string
{
    auto last_index = path.find_last_of("\\/");
    auto directory  = path.substr(0, last_index + 1);

    return directory;
}

auto FileSystem::GetExtensionFromPath(const std::string & path) -> const std::string
{
    auto last_index = path.find_last_of('.');
    if (last_index != std::string::npos)
        return path.substr(last_index, path.length());

    return NOT_ASSIGNED_STR;
}

auto FileSystem::GetPathWithoutExtension(const std::string & path) -> const std::string
{
    auto directory          = GetDirectoryFromPath(path);
    auto intact_file_name   = GetIntactFileNameFromPath(path);

    return directory + intact_file_name;
}

auto FileSystem::GetRelativeFromPath(const std::string & absolute_path) -> const std::string
{
    path p = absolute(absolute_path);
    path r = absolute(GetWorkingDirectory());

    if (p.root_path() != r.root_path())
        return p.generic_string();

    path result;

    path::const_iterator iter_path = p.begin();
    path::const_iterator iter_relative = r.begin();

    while (
        *iter_path == *iter_relative &&
        iter_path != p.end() &&
        iter_relative != r.end())
    {
        iter_path++;
        iter_relative++;
    }

    if (iter_relative != r.end())
    {
        iter_relative++;
        while (iter_relative != r.end())
        {
            result /= "..";
            iter_relative++;
        }
    }

    while (iter_path != p.end())
    {
        result /= *iter_path;
        iter_path++;
    }

    return result.generic_string();
}

auto FileSystem::GetParentDirectory(const std::string & path) -> const std::string
{
    auto found = path.find_last_of("\\/");
    auto result = path;

    if (found == std::string::npos)
        return path;

    if (found == path.length() - 1)
    {
        result = result.substr(0, found - 1);
        return GetParentDirectory(result);
    }

    return result.substr(0, found) + "/";
}

auto FileSystem::GetWorkingDirectory() -> const std::string
{
    return current_path().generic_string() + "/";
}

auto FileSystem::GetDirectoriesInDirectory(const std::string & directory) -> const std::vector<std::string>
{
    std::vector<std::string> directories;

    directory_iterator end_iter;
    for (directory_iterator iter(directory); iter != end_iter; iter++)
    {
        if (!is_directory(iter->status()))
            continue;

        directories.emplace_back(iter->path().generic_string());
    }

    return directories;
}

auto FileSystem::GetFilesInDirectory(const std::string & directory) -> const std::vector<std::string>
{
    std::vector<std::string> files;

    directory_iterator end_iter;
    for (directory_iterator iter(directory); iter != end_iter; iter++)
    {
        if (!is_regular_file(iter->status()))
            continue;

        files.emplace_back(iter->path().generic_string());
    }

    return files;
}

auto FileSystem::ToUppercase(const std::string & lower) -> const std::string
{
    std::string upper;

    for (const auto& character : lower)
        upper += toupper(character);

    return upper;
}

auto FileSystem::ToLowercase(const std::string & upper) -> const std::string
{
    std::string lower;

    for (const auto& character : upper)
        lower += tolower(character);

    return lower;
}

auto FileSystem::ToString(const std::wstring & wstr) -> const std::string
{
    return std::string();
}

auto FileSystem::ToWString(const std::string & str) -> const std::wstring
{
    std::wstring result;
    result.assign(str.begin(), str.end());

    return result;
}

void FileSystem::ReplaceAll(std::string & in_out_str, const std::string & from, const std::string & to)
{
}

auto FileSystem::IsEmptyOrWhitespace(const std::string & str) -> const bool
{
    return false;
}

auto FileSystem::IsAlphanumeric(const std::string & str) -> const bool
{
    return false;
}

auto FileSystem::GetStringBeforeExpression(const std::string & str, const std::string & exp) -> const std::string
{
    // "Hi, Hello, world", "Hello" -> "Hi, "

    return std::string();
}

auto FileSystem::GetStringAfterExpression(const std::string & str, const std::string & exp) -> const std::string
{
    // "Hi, Hello, world", "Hello" -> "world" 

    return std::string();
}

auto FileSystem::GetStringBetweenExpression(const std::string & str, const std::string & exp_a, const std::string & exp_b) -> const std::string
{
    // "Hi, Hello, world", "Hi", "world" -> "Hello" 

    return std::string();
}

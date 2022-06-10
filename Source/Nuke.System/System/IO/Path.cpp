
#include "_pch.h"
#include "Path.h"
#include <System/String.h>
#include <System/OperatingSystem.h>

namespace Nuke::System::IO
{
    namespace PathInternal
    {
#ifdef __linux__
        std::vector<char> _invalidPathChars =
        {
            '\0',
        };
        const char DirectorySeparatorChar = '/';
        const char AltDirectorySeparatorChar = '/';
        const char VolumeSeparatorChar = '/';
        const char PathSeparator = ':';
        const std::string DirectorySeparatorCharAsString = "/";
        const std::string ParentDirectoryPrefix = "../";
        bool IsDirectorySeparator(char c)
        {
            return c == DirectorySeparatorChar;
        }
        bool IsEffectivelyEmpty(const std::string& path) 
        {
            return path.empty();
        }
        int GetRootLength(const std::string& path)
        {
            return path.size() > 0 && IsDirectorySeparator(path[0]) ? 1 : 0;
        }
        bool IsPathRooted(const std::string& path)
        {
            return path.size() > 0 && path[0] == PathInternal::DirectorySeparatorChar;
        }
#elif _WIN32
        std::vector<char> _invalidPathChars =
        {
            '|', '\0',
            (char)1, (char)2, (char)3, (char)4, (char)5, (char)6, (char)7, (char)8, (char)9, (char)10,
            (char)11, (char)12, (char)13, (char)14, (char)15, (char)16, (char)17, (char)18, (char)19, (char)20,
            (char)21, (char)22, (char)23, (char)24, (char)25, (char)26, (char)27, (char)28, (char)29, (char)30,
            (char)31
        };

        const char DirectorySeparatorChar = '\\';
        const char AltDirectorySeparatorChar = '/';
        const char VolumeSeparatorChar = ':';
        const char PathSeparator = ';';

        const std::string DirectorySeparatorCharAsString = "\\";

        const std::string ExtendedPathPrefix = R"(\\?\)";
        const std::string UncPathPrefix = R"(\\)";
        const std::string UncExtendedPrefixToInsert = R"(?\UNC\)";
        const std::string UncExtendedPathPrefix = R"(\\?\UNC\)";
        const std::string DevicePathPrefix = R"(\\.\)";
        const std::string ParentDirectoryPrefix = R"(..\)";

        const int32_t MaxShortPath = 260;
        const int32_t MaxShortDirectoryPath = 248;
        // \\?\, \\.\, \??\  应对设备文件 
        const int32_t DevicePrefixLength = 4;
        // \\  应对驱动器隔离符 
        const int32_t UncPrefixLength = 2;
        // \\?\UNC\, \\.\UNC\  应对 UNC 
        const int32_t UncExtendedPrefixLength = 8;
        bool IsDirectorySeparator(char c)
        {
            return c == DirectorySeparatorChar || c == AltDirectorySeparatorChar;
        }
        bool IsEffectivelyEmpty(const std::string& path)
        {
            if (path.empty())
            {
                return true;
            }

            for (auto c : path)
            {
                if (c != ' ')
                {
                    return false;
                }
            }
            return true;
        }
        bool IsExtended(const std::string& path)
        {
            // While paths like "//?/C:/" will work, they're treated the same as "\\.\" paths.
            // Skipping of normalization will *only* occur if back slashes ('\') are used.
            return path.size() >= DevicePrefixLength
                && path[0] == '\\'
                && (path[1] == '\\' || path[1] == '?')
                && path[2] == '?'
                && path[3] == '\\';
        }
        bool IsDevice(const std::string& path)
        {
            return IsExtended(path)
                ||
                (
                    path.size() >= DevicePrefixLength
                    && IsDirectorySeparator(path[0])
                    && IsDirectorySeparator(path[1])
                    && (path[2] == '.' || path[2] == '?')
                    && IsDirectorySeparator(path[3])
                    );
        }
        bool IsDeviceUNC(const std::string& path)
        {
            return path.size() >= UncExtendedPrefixLength
                && IsDevice(path)
                && IsDirectorySeparator(path[7])
                && path[4] == 'U'
                && path[5] == 'N'
                && path[6] == 'C';
        }
        bool IsValidDriveChar(char value)
        {
            return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z');
        }
        int GetRootLength(const std::string& path)
        {
            int pathLength = path.size();
            int i = 0;

            bool deviceSyntax = IsDevice(path);
            bool deviceUnc = deviceSyntax && IsDeviceUNC(path);

            if ((!deviceSyntax || deviceUnc) && pathLength > 0 && IsDirectorySeparator(path[0]))
            {
                if (deviceUnc || (pathLength > 1 && IsDirectorySeparator(path[1])))
                {
                    i = deviceUnc ? UncExtendedPrefixLength : UncPrefixLength;

                    int n = 2;
                    while (i < pathLength && (!IsDirectorySeparator(path[i]) || --n > 0))
                    {
                        i++;
                    }
                }
                else
                {
                    i = 1;
                }
            }
            else if (deviceSyntax)
            {
                i = DevicePrefixLength;
                while (i < pathLength && !IsDirectorySeparator(path[i]))
                {
                    i++;
                }
                if (i < pathLength && i > DevicePrefixLength && IsDirectorySeparator(path[i]))
                {
                    i++;
                }
            }
            else if (pathLength >= 2
                && path[1] == VolumeSeparatorChar
                && IsValidDriveChar(path[0]))
            {
                i = 2;

                if (pathLength > 2 && IsDirectorySeparator(path[2]))
                {
                    i++;
                }
            }

            return i;
        }
        bool IsPathRooted(const std::string& path)
        {
            int length = path.size();
            return (length >= 1 && PathInternal::IsDirectorySeparator(path[0]))
                || (length >= 2 && PathInternal::IsValidDriveChar(path[0]) && path[1] == PathInternal::VolumeSeparatorChar);
        }
#elif __APPLE__
        // todo apple
    }


#endif
    bool IsCaseSensitive()
    {
        return !(OperatingSystem::IsWindows() || OperatingSystem::IsMacOS());
    }
    int GetDirectoryNameOffset(const std::string& path)
    {
        int rootLength = PathInternal::GetRootLength(path);
        int end = path.size();
        if (end <= rootLength)
        {
            return -1;
        }

        while (end > rootLength && !PathInternal::IsDirectorySeparator(path[--end]));

        while (end > rootLength && PathInternal::IsDirectorySeparator(path[end - 1]))
        {
            end--;
        }

        return end;
    }
    }

const char Path::DirectorySeparatorChar = PathInternal::DirectorySeparatorChar;
const char Path::AltDirectorySeparatorChar = PathInternal::AltDirectorySeparatorChar;
const char Path::VolumeSeparatorChar = PathInternal::VolumeSeparatorChar;
const char Path::PathSeparator = PathInternal::PathSeparator;
const std::vector<char> Path::InvalidPathChars = std::move(PathInternal::_invalidPathChars);

std::string Path::Combine(const std::initializer_list<std::string>& paths)
{
    int32_t maxSize = 0;
    int32_t firstComponent = 0;

    // 计算大小并填充

    for (auto i = 0; i < paths.size(); ++i)
    {
        const auto& path = paths.begin()[i];

        if (path.size() == 0)
        {
            continue;
        }

        if (IsPathRooted(path))
        {
            firstComponent = i;
            maxSize = path.size();
        }
        else
        {
            maxSize += path.size();
        }

        char ch = path[path.size() - 1];
        if (!PathInternal::IsDirectorySeparator(ch))
        {
            maxSize++;
        }
    }


    std::string builder;
    builder.reserve(maxSize);

    for (auto i = firstComponent; i < paths.size(); ++i)
    {
        const auto& path = paths.begin()[i];
        if (path.size() == 0)
        {
            continue;
        }

        if (builder.size() == 0)
        {
            builder.append(path);
        }
        else
        {
            char ch = builder[builder.size() - 1];
            if (!PathInternal::IsDirectorySeparator(ch))
            {
                builder.push_back(PathInternal::DirectorySeparatorChar);
            }

            builder.append(path);
        }
    }

    return builder;
}
std::string Path::Join(const std::initializer_list<std::string>& paths)
{
    if (paths.size() == 0)
    {
        return String::EmptyStdString;
    }

    int maxSize = 0;
    for (const auto& path : paths)
    {
        maxSize += path.size();
    }
    maxSize += paths.size() - 1;

    std::string builder;
    builder.reserve(maxSize);

    for (const auto& path : paths)
    {
        if (path.empty())
        {
            continue;
        }

        if (builder.size() == 0)
        {
            builder.append(path);
        }
        else
        {
            if (!PathInternal::IsDirectorySeparator(builder[builder.size() - 1]) && !PathInternal::IsDirectorySeparator(path[0]))
            {
                builder.push_back(PathInternal::DirectorySeparatorChar);
            }

            builder.append(path);
        }
    }

    return builder;
}
std::string Path::GetFileName(const std::string& path)
{
    std::size_t root = GetPathRoot(path).size();
    for (std::size_t i = path.size(); --i >= 0;)
    {
        if (i < root || PathInternal::IsDirectorySeparator(path[i]))
        {
            return path.substr(i + 1, path.size() - i - 1);
        }
    }
    return path;
}

std::string Path::GetPathRoot(const std::string& path)
{
    return IsPathRooted(path) ? PathInternal::DirectorySeparatorCharAsString : String::EmptyStdString;
}

bool Path::IsPathRooted(const std::string& path)
{
    return PathInternal::IsPathRooted(path);
}

std::string Path::GetDirectoryName(const std::string& path)
{
    if (PathInternal::IsEffectivelyEmpty(path))
    {
        return String::EmptyStdString;
    }

    int end = PathInternal::GetDirectoryNameOffset(path);
    return end >= 0 ? path.substr(0, end) : String::EmptyStdString;
}

}

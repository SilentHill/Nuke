
#pragma once

#include <System/IO/FileSystemInfo.h>

namespace Nuke::System::IO
{
    enum class SearchOption
    {
        TopDirectoryOnly,
        AllDirectories
    };

    enum class MatchType
    {
        // 使用“\* ”和“ ? ”通配符进行匹配。
        // * 从零到任意数量的字符进行匹配。 ? 与一个字符完全匹配。 * .*匹配包含句点的任何名称（使用 Win32，这将匹配所有项）。
        Simple,

        // 使用 Win32 DOS 样式匹配语义进行匹配。
        // “\* ”、“ ? ”、“<”、“>”和“"”都被视为通配符。 传统 DOS 中的匹配项 / Windows 命令提示符方式。 *.* 匹配所有文件。 ? 匹配折叠与句点。 file.??t 将匹配 file.t, file.at 和 file.txt.
        Win32
    };

    enum class MatchCasing
    {
        // 忽略字符大小写
        PlatformDefault,

        // 遵循字符大小写
        CaseSensitive,

        // 给定平台的默认大小写（如：win/linux不同）
        CaseInsensitive
    };

    class EnumerationOptions
    {
    public:
        EnumerationOptions()
        {
            SetIgnoreInaccessible(true);
            SetAttributesToSkip (FileAttributes::Hidden | FileAttributes::System);
            SetMaxRecursionDepth(DefaultMaxRecursionDepth);
        }
        static EnumerationOptions Compatible()
        {
            static EnumerationOptions enumerationOptions;
            enumerationOptions.SetMatchType(MatchType::Win32);
            enumerationOptions.SetAttributesToSkip(FileAttributes::Nothing);
            enumerationOptions.SetIgnoreInaccessible(false);
            return enumerationOptions;
        }

        static EnumerationOptions CompatibleRecursive()
        {
            static EnumerationOptions enumerationOptions;
            enumerationOptions.SetRecurseSubdirectories(true);
            enumerationOptions.SetMatchType(MatchType::Win32);
            enumerationOptions.SetAttributesToSkip(FileAttributes::Nothing);
            enumerationOptions.SetIgnoreInaccessible(false);
            return enumerationOptions;
        }

        static EnumerationOptions Default()
        {
            static EnumerationOptions enumerationOptions;
            return enumerationOptions;
        }

        static EnumerationOptions FromSearchOption(SearchOption searchOption)
        {
            if ((searchOption != SearchOption::TopDirectoryOnly) && (searchOption != SearchOption::AllDirectories))
            {
                throw std::out_of_range("search option out of range");
            }
            return searchOption == SearchOption::AllDirectories ? CompatibleRecursive() : Compatible();
        }

    public:

        const int32_t DefaultMaxRecursionDepth = std::numeric_limits<int32_t>::max();

        

        bool _recurseSubdirectories;
        bool RecurseSubdirectories()
        {
            return _recurseSubdirectories;
        }
        void SetRecurseSubdirectories(bool value)
        {
            _recurseSubdirectories = value;
        }

        bool _ignoreInaccessible;
        bool IgnoreInaccessible()
        {
            return _ignoreInaccessible;
        }
        void SetIgnoreInaccessible(bool value)
        {
            _ignoreInaccessible = value;
        }

        int32_t _bufferSize;
        int32_t BufferSize()
        {
            return _bufferSize;
        }
        void SetBufferSize(int32_t value)
        {
            _bufferSize = value;
        }

        FileAttributes _attributesToSkip;
        FileAttributes AttributesToSkip()
        {
            return _attributesToSkip;
        }
        void SetAttributesToSkip(FileAttributes value)
        {
            _attributesToSkip = value;
        }

        MatchType _matchType;
        MatchType MatchType()
        {
            return _matchType;
        }

        void SetMatchType(IO::MatchType value)
        {
            _matchType = value;
        }

        MatchCasing _matchCasing;
        MatchCasing MatchCasing()
        {
            return _matchCasing;
        }
        void SetMatchCasing(IO::MatchCasing value)
        {
            _matchCasing = value;
        }

        int32_t _maxRecursionDepth;
        int32_t MaxRecursionDepth()
        {
            return _maxRecursionDepth;
        }
        void SetMaxRecursionDepth(int32_t value)
        {
            if (value < 0)
            {
                throw std::out_of_range("max recursion out of range");
            }
            _maxRecursionDepth = value;
        }

        bool _returnSpecialDirectories;
        bool ReturnSpecialDirectories()
        {
            return _returnSpecialDirectories;
        }
        void SetReturnSpecialDirectories(bool value)
        {
            _returnSpecialDirectories = value;
        }
    };

    class Directory
    {
    public:
        Directory() = delete;
        Directory(const Directory&) = delete;
        Directory(Directory&&) = delete;
        ~Directory() = delete;

    public:
        static std::vector<std::string> EnumerateDirectories(const std::string& path);
        static std::vector<std::string> EnumerateDirectories(const std::string& path, const std::string& searchPattern, EnumerationOptions enumerationOptions);
    };
}
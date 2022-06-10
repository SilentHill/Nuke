
#pragma once

namespace Nuke::System::IO
{
    class Path
    {
    public: // 构造析构（静态类，禁止全系构造析构）
        Path() = delete;
        Path(const Path&) = delete;
        Path(Path&&) = delete;
        ~Path() = delete;

    public: // 静态属性
        static const char DirectorySeparatorChar;           // 目录分隔符 C:\\AAA  /usr/BBB
        static const char AltDirectorySeparatorChar;        // 改变目录分隔符（因Windows支持正反分隔符）
        static const char VolumeSeparatorChar;              // 磁盘卷分隔符 C:
        static const char PathSeparator;                    // 多路径并列分隔符 C:\AAA;D:\BBB
        static const std::vector<char> InvalidPathChars;    // 无法用于路径的字符

    public: // 方法

        // Combine 拼接：保证唯一根目录
        static std::string Combine(const std::initializer_list<std::string>& paths);

        // Join 拼接：纯拼接
        static std::string Join(const std::initializer_list<std::string>& paths);

        static std::string GetFileName(const std::string& path);
        static std::string GetPathRoot(const std::string& path);
        static bool IsPathRooted(const std::string& path);
        static std::string GetDirectoryName(const std::string& path);
    };
}
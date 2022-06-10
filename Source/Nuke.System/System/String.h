
#pragma once

#include "_pch.h"

namespace Nuke::System
{
    // 
    // 讲道理，引入String类是很纠结的，一方面std::string实在是弱，另一方面又想用户能够对外公用，
    // 所以对每个成员，均提供了std::string的静态等价函数，用于给std::string提供扩展，蟹蟹
    // 
    class String final : public std::string
    {
    public: // 构造析构
        String() = default;
        String(const String& str) = default;
        String(String&& str) = default;
        String(std::string&& str);
        ~String() = default;
        String(const std::string& stdStr);

        
    public: // 方法
        enum class StringComparison
        {
            CurrentCulture = 0,
            CurrentCultureIgnoreCase = 1,
            InvariantCulture = 2,
            InvariantCultureIgnoreCase = 3,
            Ordinal = 4,
            OrdinalIgnoreCase = 5,
        };
        std::vector<String> Split(const String& splitRegex) const;
        String Trim(char trimChar);
        int32_t IndexOf(char value) const;
        
    public: // 静态常量
        static const String Empty;
        static const std::string EmptyStdString;

    public: // 基类构造与操作符复用（用于完全等价于std::string）
        using std::string::string;
        using std::string::operator=;
        using std::string::operator+=;
        using std::string::operator[];

    public: // 强制引用转换，严格来说父转子不是很合适，但理论以及各编译器上表现都证明是OK的。
        static String& AsNukeString(std::string& sourceString);
        static const String& AsConstNukeString(const std::string& sourceString);

    public: // std::string静态等价函数，专门用于行为扩展
        static bool Equals(const std::string& s1, const std::string& s2, StringComparison stringComparision);
        static std::vector<std::string> Split(const std::string& sourceString, const std::string& splitRegex);
        static std::string Trim(const std::string& sourceString, char trimChar);
        static bool Contains(const std::string& sourceString, char value);
        static bool Contains(const std::string& sourceString, const std::string& value);
        static std::string Replace(const std::string& sourceString, const std::string& oldValue, const std::string& newValue);
        static std::string ConvertWstringToString(const std::wstring& wstr);
        static std::wstring ConvertStringToWstring(const std::string& str);
    };
}
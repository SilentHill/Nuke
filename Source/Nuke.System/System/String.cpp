
#include "_pch.h"
#include "String.h"
#include <locale>
#include <codecvt>

namespace Nuke::System
{
    static_assert(sizeof(String) == sizeof(std::string), "[Nuke编译期异常] 禁止在Nuke::System::String内部增加任何数据成员");
    static_assert(std::is_polymorphic<String>::value == false, "[Nuke编译期异常] 禁止在Nuke::System::String内部增加任何虚函数");

    String::String(const std::string& stdStr) : std::string(stdStr)
    {
        
    }

    String::String(std::string&& str)
    {
        *this = std::move(str);
    }

    const String String::Empty("");
    const std::string String::EmptyStdString("");

    String& String::AsString(std::string& sourceString)
    {
        return reinterpret_cast<String&>(sourceString);
    }

    const String& String::AsConstString(const std::string& sourceString)
    {
        return reinterpret_cast<const String&>(sourceString);
    }

    template <typename StringType>
    std::vector<StringType> SplitCore(const StringType& sourceString, const StringType& splitRegex)
    {
        static_assert(std::is_base_of<std::string, StringType>::value == true, "必须是std::string或其子类");
        std::regex splitRegexObject(splitRegex);
        std::sregex_token_iterator first{ sourceString.begin(), sourceString.end(), splitRegexObject, -1 }, last;
        return { first, last };
    }

    std::vector<String> String::Split(const String& splitRegex) const
    {
        return SplitCore(*this, splitRegex);
    }

    bool String::Equals(const std::string& s1, const std::string& s2, StringComparison comparisonType)
    {
        // 
        if (&s1 == &s2)
        {
            return true;
        }
        if (comparisonType == StringComparison::CurrentCultureIgnoreCase
            || comparisonType == StringComparison::InvariantCultureIgnoreCase
            || comparisonType == StringComparison::OrdinalIgnoreCase)
        {
#ifdef __linux__
            return strcasecmp(s1.c_str(), s2.c_str()) == 0;
#elif _WIN32
            return stricmp(s1.c_str(), s2.c_str()) == 0;
#elif __APPLE__
#endif
        }
        return s1 == s2;
    }

    std::vector<std::string> String::Split(const std::string& sourceString, const std::string& splitRegex)
    {
        return SplitCore(sourceString, splitRegex);
    }
    
    enum TrimType
    {
        Head = 1 << 0,
        Tail = 1 << 1,
        Both = Head | Tail
    };

    String String::Trim(char trimChar)
    {
        return String::Trim(*this, trimChar);
    }

    int32_t String::IndexOf(char value) const
    {
        return find(value, 0);
    }

    std::string CreateTrimmedString(const std::string& source, int32_t start, int32_t end)
    {
        static std::string emptyString("");
        int32_t len = end - start + 1;
        return
            len == source.size() ? source :
            len == 0 ? emptyString :
            source.substr(start, len);
    }
    std::string TrimHelper(const std::string& source, char* trimChars, int32_t trimCharsLength, TrimType trimType)
    {
        int32_t end = source.size() - 1;
        int32_t start = 0;

        if ((trimType & TrimType::Head) != 0)
        {
            for (start = 0; start < source.size(); start++)
            {
                int32_t i = 0;
                char ch = source[start];
                for (i = 0; i < trimCharsLength; i++)
                {
                    if (trimChars[i] == ch)
                    {
                        break;
                    }
                }
                if (i == trimCharsLength)
                {
                    break;
                }
            }
        }

        if ((trimType & TrimType::Tail) != 0)
        {
            for (end = source.size() - 1; end >= start; end--)
            {
                int32_t i = 0;
                char ch = source[end];
                for (i = 0; i < trimCharsLength; i++)
                {
                    if (trimChars[i] == ch)
                    {
                        break;
                    }
                }
                if (i == trimCharsLength)
                {
                    break;
                }
            }
        }

        return CreateTrimmedString(source, start, end);
    }

    std::string String::Trim(const std::string& sourceString, char trimChar)
    {
        return TrimHelper(sourceString, &trimChar, 1, TrimType::Both);
    }

    bool String::Contains(const std::string& sourceString, const std::string& value)
    {
        return sourceString.find(value) != std::string::npos;
    }
    bool String::Contains(const std::string& sourceString, char value)
    {
        return sourceString.find(value) != std::string::npos;
    }

    std::string String::Replace(const std::string& sourceString, const std::string& oldValue, const std::string& newValue)
    {
        size_t start_pos = sourceString.find(oldValue);
        if (start_pos == std::string::npos)
        {
            return sourceString;
        }
        auto ret = sourceString;
        ret.replace(start_pos, oldValue.length(), newValue);
        return ret;
    }
    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    std::string String::ConvertWstringToString(const std::wstring& wstr)
    {
        return converter.to_bytes(wstr);
    }

    std::wstring String::ConvertStringToWstring(const std::string& str)
    {
        return converter.from_bytes(str);
    }
}
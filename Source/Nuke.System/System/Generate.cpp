
#include "Generate.h"
#include "_pch.h"

namespace Nuke::System
{
    std::string Generate::RandomString(int32_t stringLength)
    {
        //static const std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        //
        //static std::uniform_int_distribution<int> distribution{ 'a', 'z' };
        //static std::random_device randomDevice;
        //static std::mt19937 generator(randomDevice());
        //std::shuffle(str.begin(), str.end(), generator);
        //return str.substr(0, stringLength);

        static std::mt19937 generator { std::random_device{}() };
        static std::uniform_int_distribution<int> distribution{ 'a', 'z' };
        std::string randomString(stringLength, '\0');

        for (auto& dis : randomString)
        {
            dis = distribution(generator);
        }
        return randomString;
    }

    std::string Generate::RandomNumberString(int32_t stringLength)
    {
        //static const std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        //
        //static std::uniform_int_distribution<int> distribution{ 'a', 'z' };
        //static std::random_device randomDevice;
        //static std::mt19937 generator(randomDevice());
        //std::shuffle(str.begin(), str.end(), generator);
        //return str.substr(0, stringLength);

        static std::mt19937 generator{ std::random_device{}() };
        static std::uniform_int_distribution<int> distribution{ '0', '9' };
        std::string randomString(stringLength, '\0');

        for (auto& dis : randomString)
        {
            dis = distribution(generator);
        }
        return randomString;
    }
}

#pragma once

#include <System/Text/Json/JsonSerializer.h>
#include <CrossPlatform/json.hpp>

namespace Nuke::System::Text::Json
{

    struct MyObject
    {
        int32_t data;
        std::vector<std::string> StringArray;
    };
    
    struct MyObjectJsonConfig
    {
        std::vector<std::string> StringArray;
    };


    using Njson = nlohmann::json;
    std::string JsonSerializer::Serialize(const IJsonSerializable& jsonSerializable)
    {
        return {};
    }

    IJsonSerializable JsonSerializer::Deserialize(const std::string& json)
    {

    }
}

#pragma once

#include <_pch.h>

namespace Nuke::System::Text::Json
{

    class IJsonSerializable
    {
    public:
        virtual std::string ToJson();
        virtual IJsonSerializable& FromJson(const std::string& json);
    };


    class JsonSerializer
    {
    public:
        std::string Serialize(const IJsonSerializable& jsonSerializable);

        IJsonSerializable Deserialize(const std::string& json);
    };
}
#pragma once

#include "CerialUtils.h"
#include "RapidJsonConverter.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <utility>

namespace Serializable 
{
    template <typename T>
    class RJObject 
    {
    public:
        virtual ~Object<T>() = default;

        static T fromJson(const typename RapidJsonConverter::JsonObj& data)
        {
            T object;

            constexpr auto sizeOfProperties = std::tuple_size<decltype(object.properties())>::value;

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i) 
            {
                constexpr auto property = std::get<i>(object.properties());

                using Type = typename decltype(property)::Type;

                if (typename RapidJsonConverter::fieldExists(data, property.name))
                {
                    object.*(property.member) = RapidJsonConverter::template toType<Type>(RapidJsonConverter::getField(data, property.name));
                }
            });

            return object;
        }

        static std::string toJsonStr() const 
        {
            auto derived = static_cast<const T&>(*this);
         
            rapidjson::StringBuffer sb;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> baseWriter;

            constexpr auto sizeOfProperties = std::tuple_size<decltype(derived.properties())>::value;

            baseWriter.StartObject();

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i) 
            {
                constexpr auto property = std::get<i>(derived.properties());

                using Type = typename decltype(property)::Type;

                RapidJsonConverter::template fromType<Type>(property.name, baseWriter, derived.*(property.member));
            });

            baseWriter.EndObject();

            return baseWriter.GetString();
        }

        static void appendJsonStr(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
        {
            auto derived = static_cast<const T&>(*this);
            constexpr auto sizeOfProperties = std::tuple_size<decltype(derived.properties())>::value;

            writer.StartObject();

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i)
            {
                constexpr auto property = std::get<i>(derived.properties());

                using Type = typename decltype(property)::Type;

                RapidJsonConverter::template fromType<Type>(property.name, writer, derived.*(property.member));
            });

            writer.EndObject();
        }
    };
}

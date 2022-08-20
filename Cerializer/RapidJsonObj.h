#pragma once

#include "CerialUtils.h"
#include "RapidJsonConverter.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <utility>

namespace Cerializer
{
    template <typename T>
    class RapidJsonObj
    {
    public:
        virtual ~RapidJsonObj<T>() = default;

        static T fromJson(const rapidjson::Document& data)
        {
            T object;

            constexpr auto sizeOfProperties = std::tuple_size<decltype(object.properties())>::value;

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i)
            {
                constexpr auto property = std::get<i>(object.properties());

                using Type = typename decltype(property)::Type;

                const auto& itr = RapidJsonConverter::getField(data, property.name);
                const bool found = itr != data.MemberEnd();

                if (property.field_check)
                {
                    if constexpr(std::is_same_v<Type, bool>)
                    {
                        object.*(property.member) = RapidJsonConverter::template toType<Type>(found);
                    }
                } 
                else if (found)
                {
                    object.*(property.member) = RapidJsonConverter::template toType<Type>(itr->value);
                }
            });

            return object;
        }

        static T fromJson(const rapidjson::GenericObject<true, rapidjson::Value> & data)
        {
            T object;

            constexpr auto sizeOfProperties = std::tuple_size<decltype(object.properties())>::value;

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i)
            {
                constexpr auto property = std::get<i>(object.properties());

                using Type = typename decltype(property)::Type;

                const auto& itr = RapidJsonConverter::getField(data, property.name);
                if (itr != data.MemberEnd())
                {
                    object.*(property.member) = RapidJsonConverter::template toType<Type>(itr->value);
                }
            });

            return object;
        }

        std::string toJsonStr() const
        {
            auto derived = static_cast<const T&>(*this);

            rapidjson::StringBuffer sb;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> baseWriter(sb);

            constexpr auto sizeOfProperties = std::tuple_size<decltype(derived.properties())>::value;

            baseWriter.StartObject();

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i)
            {
                constexpr auto property = std::get<i>(derived.properties());

                using Type = typename decltype(property)::Type;

                baseWriter.Key(property.name);
                RapidJsonConverter::template fromType<Type>(baseWriter, derived.*(property.member));
            });

            baseWriter.EndObject();

            return sb.GetString();
        }

        void appendJsonStr(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
        {
            auto derived = static_cast<const T&>(*this);
            constexpr auto sizeOfProperties = std::tuple_size<decltype(derived.properties())>::value;

            writer.StartObject();

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i)
            {
                constexpr auto property = std::get<i>(derived.properties());

                using Type = typename decltype(property)::Type;

                writer.Key(property.name);
                RapidJsonConverter::template fromType<Type>(writer, derived.*(property.member));
            });

            writer.EndObject();
        }

    private:
        RapidJsonObj() = default;
        friend T;
    };
}

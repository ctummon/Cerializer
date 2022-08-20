#pragma once

#include "SIMDJsonConverter.h"
#include <simdjson.h>

namespace Cerializer
{
    template <typename T>
    class SIMDJsonObj
    {
    public:
        virtual ~SIMDJsonObj<T>() = default;

        static T fromJson(simdjson::ondemand::parser& parser, const std::string_view data)
        {
            T object;

            auto json = simdjson::padded_string(data);
            simdjson::ondemand::document doc = parser.iterate(json); // position a pointer at the beginning of the JSON data

            auto t_el = doc["test"];

            constexpr auto sizeOfProperties = std::tuple_size<decltype(object.properties())>::value;

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i)
                {
                    constexpr auto property = std::get<i>(object.properties());

                    using Type = typename decltype(property)::Type;

                    const auto& val = SIMDJsonConverter::getField(doc, property.name);
                    //if (itr != data.MemberEnd())
                    //{
                    //    object.*(property.member) = SIMDJsonConverter::template toType<Type>(val);
                    //}
                });

            return object;
        }

        static T fromJson(const std::string_view data)
        {
            std::unique<simdjson::ondemand::parser> parser = std::make_unique<simdjson::ondemand::parser>();
            return fromJson<T>(*parser, data);
        }

        std::string_view toJsonStr() const
        {
            auto derived = static_cast<const T&>(*this);

            auto json = "{}"_padded;
            simdjson::ondemand::parser parser;
            simdjson::ondemand::document doc = parser.iterate(json);

            
            constexpr auto sizeOfProperties = std::tuple_size<decltype(derived.properties())>::value;

           //baseWriter.StartObject();

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i)
                {
                    constexpr auto property = std::get<i>(derived.properties());

                    using Type = typename decltype(property)::Type;

                    //baseWriter.Key(property.name);
                    SIMDJsonConverter::template fromType<Type>(doc, derived.*(property.member));
                });

            //baseWriter.EndObject();
           
            auto r_val = simdjson::to_json_string(doc);
            if (r_val.error()) {
                return "";
            }
            return r_val.value();
        }

        void writeToDoc(simdjson::ondemand::document& doc) const
        {
            auto derived = static_cast<const T&>(*this);
            constexpr auto sizeOfProperties = std::tuple_size<decltype(derived.properties())>::value;

            writer.StartObject();

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i)
                {
                    constexpr auto property = std::get<i>(derived.properties());

                    using Type = typename decltype(property)::Type;

                    writer.Key(property.name);
                    SIMDJsonConverter::template fromType<Type>(writer, derived.*(property.member));
                });

            writer.EndObject();
        }

    private:
        SIMDJsonObj() = default;
        friend T;
    };
}

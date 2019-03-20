#pragma once

#include "CerialUtils.h"

#include <utility>

namespace Serializable {

    template <typename T, typename Converter>
    class Object {
    public:
        virtual ~Object<T, Converter>() = default;

        static T fromJson(const typename Converter::JsonObj& data) {
            T object;

            constexpr auto sizeOfProperties = std::tuple_size<decltype(object.properties())>::value;

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {

                constexpr auto property = std::get<i>(object.properties());

                using Type = typename decltype(property)::Type;

                const auto& itr = Converter::getField(data, property.name);
                if (itr != Converter::endItr(data))
                {
                    object.*(property.member) = Converter::template toType<Type>(Converter::getValue(itr));
                }
            });

            return object;
        }

        typename Converter::JsonObj toJson() const {
            auto derived = static_cast<const T&>(*this);
            typename Converter::JsonObj data;

            constexpr auto sizeOfProperties = std::tuple_size<decltype(derived.properties())>::value;

            for_sequence(std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {

                constexpr auto property = std::get<i>(derived.properties());

                using Type = typename decltype(property)::Type;

                Converter::populateJsonObj(property.name, data, Converter::template fromType<Type>(derived.*(property.member)));
            });

            return data;
        }
    };
}

#pragma once

#include "CerialUtils.h"

#include <utility>

template<class Lambda, int=(Lambda{}(), 0)>
constexpr bool is_constexpr(Lambda) { return true; }
constexpr bool is_constexpr(...) { return false; }

namespace Cerializer{
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
                const bool found = itr != Converter::endItr(data);

                if (constexpr(is_constexpr([&property]{ property.field_check; })))
                {
                    if(std::is_same_v<decltype(object.*(property.member)), decltype(found)>)
                    {
                        object.*(property.member) = Converter::template toType<Type>(found);
                    }
                }
                else if (found)
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
} //namespace Serializable
} //namespace Cerializer

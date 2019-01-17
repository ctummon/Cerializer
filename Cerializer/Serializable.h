#pragma once

#include <utility>

template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
    using unpack_t = int[];
    (void)unpack_t {
        (static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0
    };
}

template<typename Class, typename T>
struct PropertyImpl {
    constexpr PropertyImpl(T Class::*aMember, char const* aName) : member{ aMember }, name{ aName } {}

    using Type = T;

    T Class::*member;
    char const* name;
};

template<typename Class, typename T>
constexpr auto property(T Class::*member, char const* name) {
    return PropertyImpl<Class, T>{member, name};
}

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

                if (typename Converter::fieldExists(data, property.name))
                {
                    object.*(property.member) = Converter::template toType<Type>(Converter::getField(data, property.name));
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

#define S_PROPERTIES_BEGIN \
constexpr static auto properties() { \
return std::make_tuple( \

#define S_PROPERTIES_END \
); \
} \

#define S_PROPERTY(ClassName, Name)  \
property(&ClassName::Name, #Name )

#pragma  once

#include <locale>
#include <codecvt>
#include <string>
#include <tuple>

template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
    using unpack_t = int[];
    (void)unpack_t {
        (static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0
    };
}

template<typename Class, typename T, typename IdentiferType>
struct PropertyImpl {
    constexpr PropertyImpl(T Class::*aMember, IdentiferType const* aName) : member{ aMember }, name{ aName } {}

    using Type = T;

    T Class::*member;
    IdentiferType const* name;
};

template<typename Class, typename T, typename IdentiferType>
constexpr auto property(T Class::*member, IdentiferType const* name) {
    return PropertyImpl<Class, T, IdentiferType>{member, name};
}

#define S_PROPERTIES_BEGIN \
constexpr static auto properties() { \
return std::make_tuple( \

#define S_PROPERTIES_END \
); \
} \

#define S_PROPERTY(ClassName, Name)  \
property(&ClassName::Name, #Name )


#ifdef _WIN32
#define CPPREST_S_PROPERTY(ClassName, Name)  \
    property(&ClassName::Name, L#Name)
#else
#define CPPREST_S_PROPERTY(ClassName, Name)  \
    property(&ClassName::Name, #Name)
#endif

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> stringConverter;
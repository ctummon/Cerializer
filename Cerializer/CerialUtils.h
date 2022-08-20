#pragma once

#include <locale>
#include <codecvt>
#include <string>
#include <tuple>

namespace Cerializer {
 
template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
    using unpack_t = int[];
    (void)unpack_t {
        (static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0
    };
}

template<typename Class, typename T, typename IdentiferType>
struct PropertyImpl {
    constexpr PropertyImpl(T Class::* aMember, IdentiferType const* aName, bool aField_check = false) 
        : member{ aMember }
        , name{ aName }
        , field_check{ aField_check } 
    {}

    using Type = T;

    T Class::*member;
    IdentiferType const* name;
    bool field_check;
};

template<typename Class, typename T, typename IdentiferType>
constexpr auto property(T Class::*member, IdentiferType const* name, bool field_check = false) {
    return PropertyImpl<Class, T, IdentiferType>{member, name, field_check};
}

#define S_PROPERTIES_BEGIN(ClassName) \
private: \
using HiddenClassNameAlias = ClassName; \
public: \
constexpr static auto properties() { \
return std::make_tuple( \

#define S_PROPERTIES_END \
); \
} \

#define S_PROPERTY(Var)  \
property(&HiddenClassNameAlias::Var, #Var )

#define S_FIELD_CHECK(Var)  \
property(&HiddenClassNameAlias::Var, #Var, true )

#ifdef _WIN32
#define CPPREST_S_PROPERTY(Var)  \
property(&HiddenClassNameAlias::Var, L#Var)

#define CPPREST_FIELD_CHECK(Var)  \
property(&HiddenClassNameAlias::Var, L#Var, true )
#else
#define CPPREST_S_PROPERTY(Var)  \
property(&HiddenClassNameAlias::Var, #Var)

#define CPPREST_FIELD_CHECK(Name)  \
property(&HiddenClassNameAlias::Name, L#Name, true )
#endif

static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> stringConverter;

} //namespace Cerializer
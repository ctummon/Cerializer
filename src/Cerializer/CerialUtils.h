#pragma once

#include <codecvt>
#include <locale>

namespace Cerializer {
namespace Utils {

template<typename T, T... S, typename F>
constexpr void
for_sequence(std::integer_sequence<T, S...>, F&& f)
{
    using unpack_t = int[];
    (void)unpack_t{
        (static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0
    };
}

#define S_PROPERTIES_BEGIN                                                     \
    constexpr static auto getProperties()                                      \
    {                                                                          \
return std::make_tuple(

#define S_PROPERTIES_END                                                       \
);                                                                             \
    }

#define S_PROPERTY(Var) property(&DerivedClassAlias::Var, #Var)

#define S_FIELD_CHECK(Var) property(&DerivedClassAlias::Var, #Var, true)

#ifdef _WIN32
#define CPPREST_S_PROPERTY(Var) property(&DerivedClassAlias::Var, L#Var)

#define CPPREST_FIELD_CHECK(Var) property(&DerivedClassAlias::Var, L#Var, true)
#else
#define CPPREST_S_PROPERTY(Var) property(&DerivedClassAlias::Var, #Var)

#define CPPREST_FIELD_CHECK(Name)                                              \
    property(&DerivedClassAlias::Name, L#Name, true)
#endif

static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>
  stringConverter;

} // namespace Utils
} // namespace Cerializer
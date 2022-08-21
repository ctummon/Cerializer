#pragma once

#include <utility>

#include "CerialUtils.h"
#include "CerializerProperties.h"

namespace Cerializer {
namespace Serializable {

template<typename DerivedClass, typename Converter>
class Object : public Properties<DerivedClass>
{
  public:
    virtual ~Object<DerivedClass, Converter>() = default;

    static DerivedClass fromJson(const typename Converter::JsonObj& data)
    {
        DerivedClass object;

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(object.getProperties())>::value;

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(object.getProperties());

              using Type = typename decltype(property)::Type;

              const auto& itr = Converter::getField(data, property.name);
              const bool found = itr != Converter::endItr(data);

              if (property.field_check) {
                  if constexpr (std::is_same_v<Type, bool>) {
                      object.*(property.member) =
                        Converter::template toType<Type>(found);
                  }
              } else if (found) {
                  object.*(property.member) =
                    Converter::template toType<Type>(Converter::getValue(itr));
              }
          });

        return object;
    }

    typename Converter::JsonObj toJson() const
    {
        const auto derived = static_cast<const DerivedClass&>(*this);
        typename Converter::JsonObj data;

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(derived.getProperties())>::value;

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(derived.getProperties());

              using Type = typename decltype(property)::Type;

              Converter::populateJsonObj(property.name,
                data,
                Converter::template fromType<Type>(derived.*(property.member)));
          });

        return data;
    }
};
} // namespace Serializable
} // namespace Cerializer

#pragma once

#include "CerialUtils.h"
#include "CerializerProperties.h"
#include "RapidJsonConverter.h"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <utility>

namespace Cerializer {
template<typename DerivedClass>
class RapidJsonObj : public Properties<DerivedClass>
{
  public:
    virtual ~RapidJsonObj<DerivedClass>() = default;

    static DerivedClass fromJson(const rapidjson::Document& data)
    {
        DerivedClass object;

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(object.getProperties())>::value;

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(object.getProperties());

              using Type = typename decltype(property)::Type;

              const auto& itr =
                RapidJsonConverter::getField(data, property.name);
              if (itr != data.MemberEnd()) {
                  object.*(property.member) =
                    RapidJsonConverter::template toType<Type>(itr->value);
              }
          });

        return object;
    }

    static DerivedClass fromJson(
      const rapidjson::GenericObject<true, rapidjson::Value>& data)
    {
        DerivedClass object;

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(object.getProperties())>::value;

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(object.getProperties());

              using Type = typename decltype(property)::Type;

              const auto& itr =
                RapidJsonConverter::getField(data, property.name);
              if (itr != data.MemberEnd()) {
                  object.*(property.member) =
                    RapidJsonConverter::template toType<Type>(itr->value);
              }
          });

        return object;
    }

    std::string toJsonStr() const
    {
        const auto derived = static_cast<const DerivedClass&>(*this);

        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> baseWriter(sb);

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(derived.getProperties())>::value;

        baseWriter.StartObject();

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(derived.getProperties());

              using Type = typename decltype(property)::Type;

              baseWriter.Key(property.name);
              RapidJsonConverter::template fromType<Type>(
                baseWriter, derived.*(property.member));
          });

        baseWriter.EndObject();

        return sb.GetString();
    }

    void appendJsonStr(
      rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
    {
        const auto derived = static_cast<const DerivedClass&>(*this);
        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(derived.getProperties())>::value;

        writer.StartObject();

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(derived.getProperties());

              using Type = typename decltype(property)::Type;

              writer.Key(property.name);
              RapidJsonConverter::template fromType<Type>(
                writer, derived.*(property.member));
          });

        writer.EndObject();
    }

  private:
    RapidJsonObj() = default;
    friend DerivedClass;
};
} // namespace Cerializer

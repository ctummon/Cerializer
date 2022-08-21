#pragma once

#include <simdjson.h>

#include "CerializerProperties.h"
#include "SimdJsonConverter.h"

namespace Cerializer {
template<typename DerivedClass>
class SimdJsonObj : public Properties<DerivedClass>
{
  public:
    virtual ~SimdJsonObj<DerivedClass>() = default;

    static DerivedClass fromJson(simdjson::ondemand::parser& parser,
      const std::string_view data)
    {
        DerivedClass object;

        auto json = simdjson::padded_string(data);
        simdjson::ondemand::document doc = parser.iterate(
          json); // position a pointer at the beginning of the JSON data

        auto t_el = doc["test"];

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(object.getProperties())>::value;

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(object.getProperties());

              using Type = typename decltype(property)::Type;

              const auto& val = SimdJsonConverter::getField(doc, property.name);
              // if (itr != data.MemberEnd())
              //{
              //    object.*(property.member) = SimdJsonConverter::template
              //    toType<Type>(val);
              //}
          });

        return object;
    }

    static DerivedClass fromJson(const std::string_view data)
    {
        std::unique<simdjson::ondemand::parser> parser =
          std::make_unique<simdjson::ondemand::parser>();
        return fromJson<T>(*parser, data);
    }

    std::string_view toJsonStr() const
    {
        auto derived = static_cast<const DerivedClass&>(*this);

        auto json = "{}"_padded;
        simdjson::ondemand::parser parser;
        simdjson::ondemand::document doc = parser.iterate(json);

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(derived.getProperties())>::value;

        // baseWriter.StartObject();

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(derived.getProperties());

              using Type = typename decltype(property)::Type;

              // baseWriter.Key(property.name);
              SimdJsonConverter::template fromType<Type>(
                doc, derived.*(property.member));
          });

        // baseWriter.EndObject();

        auto r_val = simdjson::to_json_string(doc);
        if (r_val.error()) {
            return "";
        }
        return r_val.value();
    }

    void writeToDoc(simdjson::ondemand::document& doc) const
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
              SimdJsonConverter::template fromType<Type>(
                writer, derived.*(property.member));
          });

        writer.EndObject();
    }

  private:
    SimdJsonObj() = default;
    friend DerivedClass;
};
} // namespace Cerializer

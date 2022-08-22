#pragma once

#include "CerializerProperties.h"
#include "SimdJsonConverter.h"

#include <simdjson.h>

#include <cstring>
#include <type_traits>
#include <optional>

#include <iostream>//remove when done testing

// TODO: Need to ensure only 1 per thread, not thread safe.
static simdjson::ondemand::parser&
getSimdJsonParser()
{
    static simdjson::ondemand::parser simdjson_parser;
    return simdjson_parser;
}

namespace Cerializer {

template<typename DerivedClass>
class SimdJsonObj : public Properties<DerivedClass>

{

template<typename MemberObject>
using IsNotMemberObject = std::negation<std::is_base_of<SimdJsonObj<MemberObject>, MemberObject>>;

template<typename MemberObject>
using IsNotSharedPtrMemberObject = std::negation<std::is_same<std::shared_ptr<typename MemberObject::element_type>, MemberObject>>;

template<typename MemberObject>
using IsNotOptionalMemberObject = std::negation<std::is_same<std::optional<typename MemberObject::value_type>, MemberObject>>;

template<typename MemberObject>
using ShouldNotHandleObject = std::conjunction<
    IsNotMemberObject<MemberObject>,
    IsNotSharedPtrMemberObject<MemberObject>,
    IsNotOptionalMemberObject<MemberObject>>;

  public:
    virtual ~SimdJsonObj<DerivedClass>() = default;

    template<typename JsonValue, typename ParentClass>
    static void writeToMemberField(
      const simdjson::ondemand::raw_json_string keyName,
      const JsonValue simdJsonVal,
      ParentClass& object)
    {
        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(object.getProperties())>::value;

        // This approach may be slow, need to compare with simdjson DOM Api
        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(object.getProperties());
              using Type = typename decltype(property)::Type;
              if (keyName == property.name) {
                  object.*(property.member) =
                    SimdJsonConverter::template toType<Type>(simdJsonVal);
              }
          });
    }

    template<typename MemberObject, typename std::enable_if<std::is_base_of_v<SimdJsonObj<MemberObject>,MemberObject>>::type* = nullptr>
    static bool writeObjectToFieldImpl(
      simdjson::fallback::ondemand::object jsonObj,
      MemberObject& memberObject)
      {
        for (auto field : jsonObj) {
            if (!field.key().error() && !field.value().error()) {
                recursive_processor(field.key(), field.value(), memberObject);
            }
        }
        return true;
      }

    template<typename MemberObject, typename std::enable_if<IsNotMemberObject<MemberObject>::value>::type* = nullptr>
    static bool writeObjectToFieldImpl(
      simdjson::fallback::ondemand::object jsonObj,
      MemberObject& memberObject)
      {
        //If we don't inherit SimdJsonObj do nothing and return not handled.
        return false;
      }

    template<typename MemberObject, typename std::enable_if<std::is_base_of_v<SimdJsonObj<MemberObject>,MemberObject>>::type* = nullptr>
    static void filterObject(simdjson::ondemand::object jsonObj, MemberObject& memberObject)
    {
        writeObjectToFieldImpl(jsonObj, memberObject);
    }

    template<typename MemberObject, typename std::enable_if<std::is_same_v<std::optional<typename MemberObject::value_type>, MemberObject>>::type* = nullptr>
    static void filterObject(simdjson::ondemand::object jsonObj,
      std::optional<typename MemberObject::value_type>& memberObject)
    {
        MemberObject::value_type innerType;
        if(writeObjectToFieldImpl(jsonObj, innerType)){
            memberObject = std::make_optional<MemberObject::value_type>(innerType);
        }
    }

    template<typename MemberObject,
      typename std::enable_if<std::is_same_v<std::shared_ptr<typename MemberObject::element_type>, MemberObject>>::type* = nullptr>
    static void filterObject(simdjson::ondemand::object jsonObj,
      std::shared_ptr<typename MemberObject::element_type>& memberObject)
    {
        MemberObject::element_type innerType;
        if(writeObjectToFieldImpl(jsonObj, innerType)){
            memberObject = std::make_shared<MemberObject::element_type>(innerType);
        }
    }

    template<typename MemberObject, typename std::enable_if<IsNotMemberObject<MemberObject>::value>::type* = nullptr>
    static void filterObject(simdjson::ondemand::object jsonObj, MemberObject& memberObject)
    {
        //No-op does not inherit SimdJsonObj
    }

    template<typename ParentObject>
    static void writeObjectToField(
      const simdjson::ondemand::raw_json_string keyName,
      simdjson::simdjson_result<simdjson::ondemand::object> objectResult,
      ParentObject& parentObject)
    {
        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(parentObject.getProperties())>::value;

        if (objectResult.error()) {
            return;
        }
        auto objVal = objectResult.value_unsafe();

        // This approach may be slow, need to compare with simdjson DOM Api
        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property =
                std::get<i>(parentObject.getProperties());
                using Type = typename decltype(property)::Type;
                if (keyName == property.name) {
                    filterObject<Type>(objVal, parentObject.*(property.member));
                }
          });
    }

    template<class ParentClass>
    static void recursive_processor(
      simdjson::ondemand::raw_json_string key,
      simdjson::ondemand::value element,
      ParentClass& parentClass)
    {
        switch (element.type()) {
            case simdjson::ondemand::json_type::object:
                if (key.raw() == nullptr) {
                    for (auto field : element.get_object()) {
                        if (!field.key().error() && !field.value().error()) {
                            recursive_processor(field.key(),
                              field.value(),
                              parentClass);
                        }
                    }
                } else {
                    writeObjectToField(key, element.get_object(), parentClass);
                }
                break;
            case simdjson::ondemand::json_type::number:
            case simdjson::ondemand::json_type::string:
            case simdjson::ondemand::json_type::boolean:
                writeToMemberField(key, element, parentClass);
                break;
            case simdjson::ondemand::json_type::null:
                break;
            default:
                break;
        }
    }

    static void recursive_processor(simdjson::ondemand::value element,
      DerivedClass& derivedClassObj)
    {
        recursive_processor(simdjson::ondemand::raw_json_string(), element, derivedClassObj);
    }

    static DerivedClass fromJson(const std::string_view& data)
    {
        DerivedClass object;
        auto paddedStrV = simdjson::padded_string(data);
        simdjson::ondemand::document doc =
          getSimdJsonParser().iterate(paddedStrV);
        // we don't support scalar json objects.
        if (!doc.is_scalar()) {
            simdjson::ondemand::value val = doc;
            recursive_processor(val, object);
        }
        return object;
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

#pragma once

#include "CerializerProperties.h"
#include "SimdJsonConverter.h"

#include <minijson_writer/minijson_writer.hpp>
#include <simdjson.h>

#include <cstring>
#include <optional>
#include <type_traits>
#include <set>
#include <vector>

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
    using IsNotMemberObject =
      std::negation<std::is_base_of<SimdJsonObj<MemberObject>, MemberObject>>;

    template<typename MemberObject>
    using IsNotSharedPtrMemberObject = std::negation<
      std::is_same<std::shared_ptr<typename MemberObject::element_type>,
        MemberObject>>;

    template<typename MemberObject>
    using IsNotOptionalMemberObject = std::negation<
      std::is_same<std::optional<typename MemberObject::value_type>,
        MemberObject>>;

    template<typename MemberObject>
    using ShouldNotHandleObject =
      std::conjunction<IsNotMemberObject<MemberObject>,
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

    template<typename MemberObject,
      typename std::enable_if<std::is_base_of_v<SimdJsonObj<MemberObject>,
        MemberObject>>::type* = nullptr>
    static bool writeObjectToFieldImpl(
      simdjson::fallback::ondemand::object jsonObj,
      MemberObject& memberObject)
    {
        for (auto field : jsonObj) {
            if (!field.key().error() && !field.value().error()) {
                recursive_processor(memberObject, field.value(), field.key());
            }
        }
        return true;
    }

    template<typename MemberObject,
      typename std::enable_if<IsNotMemberObject<MemberObject>::value>::type* =
        nullptr>
    static bool writeObjectToFieldImpl(
      simdjson::fallback::ondemand::object jsonObj,
      MemberObject& memberObject)
    {
        // If we don't inherit SimdJsonObj do nothing and return not handled.
        return false;
    }

    template<typename MemberObject,
      typename std::enable_if<std::is_base_of_v<SimdJsonObj<MemberObject>,
        MemberObject>>::type* = nullptr>
    static void filterObject(simdjson::ondemand::object jsonObj,
      MemberObject& memberObject)
    {
        writeObjectToFieldImpl(jsonObj, memberObject);
    }

    template<typename MemberObject,
      typename std::enable_if<
        std::is_same_v<std::optional<typename MemberObject::value_type>,
          MemberObject>>::type* = nullptr>
    static void filterObject(simdjson::ondemand::object jsonObj,
      std::optional<typename MemberObject::value_type>& memberObject)
    {
        MemberObject::value_type innerType;
        if (writeObjectToFieldImpl(jsonObj, innerType)) {
            memberObject =
              std::make_optional<MemberObject::value_type>(innerType);
        }
    }

    template<typename MemberObject,
      typename std::enable_if<
        std::is_same_v<std::shared_ptr<typename MemberObject::element_type>,
          MemberObject>>::type* = nullptr>
    static void filterObject(simdjson::ondemand::object jsonObj,
      std::shared_ptr<typename MemberObject::element_type>& memberObject)
    {
        MemberObject::element_type innerType;
        if (writeObjectToFieldImpl(jsonObj, innerType)) {
            memberObject =
              std::make_shared<MemberObject::element_type>(innerType);
        }
    }

    template<typename MemberObject,
      typename std::enable_if<IsNotMemberObject<MemberObject>::value>::type* =
        nullptr>
    static void filterObject(simdjson::ondemand::object jsonObj,
      MemberObject& memberObject)
    {
        //No-op
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

    template<typename ParentObject>//, typename std::enable_if<std::is_same<std::vector<int>, MemberObject>::type>::type* = nullptr>
    static void handleArray(
      ParentObject& parentObject,
      simdjson::ondemand::value jsonVal,
      simdjson::ondemand::raw_json_string keyName)
    {
        auto ar = jsonVal.get_array();
        if (ar.error()) {
            return;
        }
        auto arrayValue = ar.value_unsafe();

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(parentObject.getProperties())>::value;

        // This approach may be slow, need to compare with simdjson DOM Api
        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property =
                std::get<i>(parentObject.getProperties());
              using Type = typename decltype(property)::Type;
              if (keyName == property.name) {
                for (auto child : arrayValue) {
                    //Decide whether to push back fundamental value or object or container.
                    //To either vector or set.
                    //parentObject.*(property.member).push_back(SimdJsonConverter::template toType<Type>(jsonVal));
                }
              }
          });
    }

    template<typename MemberObject>
    using IsMemberObjectSet = std::is_same<MemberObject, std::set<typename MemberObject::value_type>>;

    /*template<typename MemberObject, typename std::enable_if<IsMemberObjectSet<MemberObject>::value>::type* = nullptr>
    static void writeToArray(
      std::set<typename MemberObject>& memberObject,
      simdjson::ondemand::value jsonVal,
      simdjson::ondemand::raw_json_string key)
    {
        auto ar = jsonVal.get_array();
        if (ar.error()) {
            return;
        }
        auto a = ar.value_unsafe();
        for (auto child : a) {
            MemberObject::value_type newType;
            recursive_processor(newType, child);
            memberObject.insert(toType<typename T::value_type>(newType))
        }
    }*/

    template<typename MemberObject>
    using IsNotMemberObjectSet = std::negation<IsMemberObjectSet<MemberObject>>;

    template<typename MemberObject>
    using IsNotMemberObjectVector = std::negation<std::is_same<MemberObject, std::vector<typename MemberObject::value_type>>>;

    template<typename MemberObject>
    using IsNotVectorOrSet = std::conjunction<IsNotMemberObjectSet<MemberObject>, IsNotMemberObjectVector<MemberObject>>;

    /*template<typename MemberObject, typename std::enable_if<IsNotMemberObjectSet<MemberObject>::value>::type* = nullptr >
    static void writeToArray(MemberObject& memberObject,
      simdjson::ondemand::value jsonVal,
      simdjson::ondemand::raw_json_string key)
    {
        // No-op
    }*/

    template<class ParentObject>
    static void recursive_processor(ParentObject& parentObject,
      simdjson::ondemand::value element,
      simdjson::ondemand::raw_json_string key =
        simdjson::ondemand::raw_json_string())
    {
        switch (element.type()) {
            case simdjson::ondemand::json_type::object:
                if (key.raw() == nullptr) {
                    for (auto field : element.get_object()) {
                        if (!field.key().error() && !field.value().error()) {
                            recursive_processor(
                              parentObject, field.value(), field.key());
                        }
                    }
                } else {
                    writeObjectToField(key, element.get_object(), parentObject);
                }
                break;
            case simdjson::ondemand::json_type::array:
                handleArray(parentObject, element, key);
                break;
            case simdjson::ondemand::json_type::number:
            case simdjson::ondemand::json_type::string:
            case simdjson::ondemand::json_type::boolean:
                writeToMemberField(key, element, parentObject);
                break;
            case simdjson::ondemand::json_type::null:
                break;
            default:
                break;
        }
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
            recursive_processor(object, val);
        }
        return object;
    }

    void serializeToWriter(minijson::object_writer& writer) const
    {
        auto derived = static_cast<const DerivedClass&>(*this);

        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(derived.getProperties())>::value;

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(derived.getProperties());

              using Type = typename decltype(property)::Type;

              SimdJsonConverter::template fromType<Type>(
                writer, derived.*(property.member), property.name);
          });
    }

    std::string toJsonStr() const
    {
        std::stringstream ss;
        minijson::object_writer writer(ss); // wrap a std::ostream
        serializeToWriter(writer);
        writer.close();
        return ss.str();
    }

  private:
    SimdJsonObj() = default;
    friend DerivedClass;
};
} // namespace Cerializer

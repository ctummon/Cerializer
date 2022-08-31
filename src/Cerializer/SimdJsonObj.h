#pragma once

#include "CerializerProperties.h"
#include "SimdJsonConverter.h"

#include <minijson_writer/minijson_writer.hpp>
#include <simdjson.h>

#include <cstring>
#include <optional>
#include <set>
#include <type_traits>
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

  public:
    virtual ~SimdJsonObj<DerivedClass>() = default;

    /////// De-serialize /////////////
    template<class T,
      typename std::enable_if<
        std::is_same<std::optional<typename T::value_type>, T>::value>::type* =
        nullptr>
    static T toType(simdjson::ondemand::value val)
    {
        return std::make_optional<T::value_type>(toType<T::value_type>(val));
    }

    template<class T,
      typename std::enable_if<
        std::is_base_of<Cerializer::SimdJsonObj<T>, T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        T returnType;

        recursive_processor(returnType, v);

        return returnType;
    }

    template<class T,
      typename std::enable_if<std::is_same<short, T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        short returnVal{};

        auto ir = v.get_int64();
        if (!ir.error()) {
            returnVal = static_cast<short>(ir.value_unsafe());
        }

        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned short, T>::value>::type* =
        nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        unsigned short returnVal{};

        auto ir = v.get_int64();
        if (!ir.error()) {
            returnVal = static_cast<unsigned short>(ir.value_unsafe());
        }

        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<int, T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        int returnVal{};
        auto ir = v.get_int64();
        if (!ir.error()) {
            returnVal = static_cast<int>(ir.value_unsafe());
        }

        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned int, T>::value>::type* =
        nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        unsigned int returnVal{};
        auto ir = v.get_int64();
        if (!ir.error()) {
            returnVal = static_cast<unsigned int>(ir.value_unsafe());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<long, T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        long returnVal{};
        auto ir = v.get_int64();
        if (!ir.error()) {
            returnVal = static_cast<long>(ir.value_unsafe());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned long, T>::value>::type* =
        nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        unsigned long returnVal{};
        auto ir = v.get_int64();
        if (!ir.error()) {
            returnVal = static_cast<unsigned long>(ir.value_unsafe());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<long long, T>::value>::type* =
        nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        long long returnVal{};
        auto ir = v.get_int64();
        if (!ir.error()) {
            returnVal = static_cast<long long>(ir.value_unsafe());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::wstring, T>::value>::type* =
        nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        T returnVal;
        auto sr = v.get_string();
        if (!sr.error()) {
            const auto stringView = sr.value_unsafe();
            returnVal = Utils::stringConverter.from_bytes(
              stringView.data(), stringView.data() + stringView.size());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::string, T>::value>::type* =
        nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        T returnVal;
        auto sr = v.get_string();
        if (!sr.error()) {
            returnVal = sr.value_unsafe();
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<double, T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        double returnVal{};
        auto dr = v.get_double();
        if (!dr.error()) {
            returnVal = static_cast<double>(dr.value_unsafe());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<float, T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        float returnVal{};
        auto fr = v.get_double();
        if (!fr.error()) {
            returnVal = static_cast<float>(fr.value_unsafe());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<bool, T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        bool returnVal{};
        auto br = v.get_bool();
        if (!br.error()) {
            returnVal = br.value_unsafe();
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<
        std::is_same<std::shared_ptr<typename T::element_type>,
          T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        return std::make_shared<T::element_type>(toType<T::element_type>(v));
    }

    template<class T,
      typename std::enable_if<std::is_same<std::vector<typename T::value_type>,
        T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        T returnArray;

        auto ar = v.get_array();
        if (!ar.error()) {
            auto arrayValue = ar.value_unsafe();
            for (auto child : arrayValue) {
                if (!child.error()) {
                    returnArray.push_back(
                      toType<typename T::value_type>(child.value_unsafe()));
                }
            }
        }

        return returnArray;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::set<typename T::value_type>,
        T>::value>::type* = nullptr>
    static T toType(simdjson::ondemand::value v)
    {
        T returnSet;

        auto ar = v.get_array();
        if (!ar.error()) {
            auto arrayValue = ar.value_unsafe();
            for (auto child : arrayValue) {
                if (!child.error()) {
                    returnSet.insert(
                      toType<typename T::value_type>(child.value_unsafe()));
                }
            }
        }

        return returnSet;
    }

    /////////////////////////////////////
    template<typename JsonValue, typename ParentClass>
    static void writeToMemberField(
      const simdjson::ondemand::raw_json_string keyName,
      const JsonValue simdJsonVal,
      ParentClass& object)
    {
        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(object.getProperties())>::value;

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property = std::get<i>(object.getProperties());
              using Type = typename decltype(property)::Type;
              if (keyName == property.name) {
                  object.*(property.member) = toType<Type>(simdJsonVal);
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
        // No-op
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

    template<
      typename ParentObject> //, typename
                             // std::enable_if<std::is_same<std::vector<int>,
                             // MemberObject>::type>::type* = nullptr>
    static void handleArray(ParentObject& parentObject,
      simdjson::ondemand::value jsonVal,
      simdjson::ondemand::raw_json_string keyName)
    {
        constexpr auto sizeOfProperties =
          std::tuple_size<decltype(parentObject.getProperties())>::value;

        Utils::for_sequence(
          std::make_index_sequence<sizeOfProperties>{}, [&](auto i) {
              constexpr auto property =
                std::get<i>(parentObject.getProperties());
              using Type = typename decltype(property)::Type;
              if (keyName == property.name) {
                  parentObject.*(property.member) = toType<Type>(jsonVal);
              }
          });
    }

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
        minijson::object_writer writer(ss);
        serializeToWriter(writer);
        writer.close();
        // be nice if this could be replaced with something that doesn't make an
        // extra copy.
        return ss.str();
    }

  private:
    SimdJsonObj() = default;
    friend DerivedClass;
};
} // namespace Cerializer

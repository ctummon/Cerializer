#pragma once

#include "CerialUtils.h"

#include <minijson_writer/minijson_writer.hpp>
#include <simdjson.h>

#include <optional>
#include <set>
#include <type_traits>
#include <vector>

namespace Cerializer {
template<typename>
class SimdJsonObj;

struct SimdJsonConverter
{
    using JsonVal = simdjson::ondemand::value;

    // De-serialize
    template<class T,
      typename std::enable_if<
        std::is_same<std::optional<typename T::value_type>, T>::value>::type* =
        nullptr>
    static T toType(JsonVal val)
    {
        return std::make_optional<T::value_type>(toType<T::value_type>(val));
    }

    template<class T,
      typename std::enable_if<
        std::is_base_of<Cerializer::SimdJsonObj<T>, T>::value>::type* = nullptr>
    static T toType(JsonVal val)
    {
        // TODO: Refactor how SimdJsonConverter and SimdJsonObj interact.
        assert("Not supported");
        return T{};
    }

    template<class T,
      typename std::enable_if<std::is_same<short, T>::value>::type* = nullptr>
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
    {
        T returnVal;
        auto sr = v.get_string();
        if (!sr.error()) {
            auto stringView = sr.value_unsafe();
            // todo: fix
            returnVal = Utils::stringConverter.from_bytes(
              &*stringView.begin(), &*stringView.begin());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::string, T>::value>::type* =
        nullptr>
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
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
    static T toType(JsonVal v)
    {
        // Should be handled elsewhere, need to refactor
        assert("Not supported");
        return std::make_shared<T::element_type>();
    }

    template<class T,
      typename std::enable_if<std::is_same<std::vector<typename T::value_type>,
        T>::value>::type* = nullptr>
    static T toType(JsonVal v)
    {
        T returnArray;
        // need to account for array of objects. Whole class needs a refactor.
        /*if (v.IsArray())
        {
            const auto& arrayData = v.GetArray();
            const auto& endItr = arrayData.end();
            auto itr = arrayData.begin();
            for (; itr != endItr; ++itr)
            {
                returnArray.push_back(toType<typename T::value_type>(*itr));
            }
        }*/
        return returnArray;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::set<typename T::value_type>,
        T>::value>::type* = nullptr>
    static T toType(JsonVal v)
    {
        T returnSet;
        /*if (v.IsArray())
    {
        const auto& arrayData = v.GetArray();
        const auto& endItr = arrayData.end();
        auto itr = arrayData.begin();
        for (; itr != endItr; ++itr)
        {
            returnSet.insert(toType<typename T::value_type>(*itr));
        }
    }*/
        return returnSet;
    }

    // Serialize
    // Object writers
    template<class T>
    static void fromType(minijson::object_writer& writer,
      Cerializer::SimdJsonObj<T>& val,
      const char* key)
    {
        auto nested_writer = writer.nested_object(key);
        val.serializeToWriter(nested_writer);
        nested_writer.close();
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const Cerializer::SimdJsonObj<T>& val,
      const char* key)
    {
        auto nested_writer = writer.nested_object(key);
        val.serializeToWriter(nested_writer);
        nested_writer.close();
    }

    template<class T,
      typename BasicType,
      typename std::enable_if_t<std::is_fundamental_v<T>>>
    static void fromType(minijson::object_writer& writer,
      const BasicType val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const short val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const unsigned short val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const int val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const unsigned int val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const long val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const unsigned long val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const long long val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const double val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const float val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const bool val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const std::string& val,
      const char* key)
    {
        writer.write(key, val);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const std::wstring& val,
      const char* key)
    {
        writer.write(key, Utils::stringConverter.to_bytes(val));
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const std::shared_ptr<typename T::element_type>& ptr,
      const char* key)
    {
        if (ptr) {
            fromType<T::element_type>(writer, *ptr, key);
        }
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      typename T::element_type* ptr,
      const char* key)
    {
        if (ptr) {
            fromType<T::element_type>(writer, ptr, key);
        }
    }

    // Array writers
    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      Cerializer::SimdJsonObj<T>& val)
    {
        auto nested_writer = writer.nested_object();
        val.serializeToWriter(nested_writer);
        nested_writer.close();
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      const Cerializer::SimdJsonObj<T>& val)
    {
        auto nested_writer = writer.nested_object();
        val.serializeToWriter(nested_writer);
        nested_writer.close();
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer, const short val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      const unsigned short val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer, const int val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      const unsigned int val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer, const long val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      const unsigned long val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      const long long val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer, const double val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer, const float val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer, const bool val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      const std::string& val)
    {
        writer.write(val);
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      const std::wstring& val)
    {
        writer.write(Utils::stringConverter.to_bytes(val));
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      const std::shared_ptr<typename T::element_type>& ptr)
    {
        if (ptr) {
            fromType<T::element_type>(writer, *ptr);
        }
    }

    template<class T>
    static void fromTypeArray(minijson::array_writer& writer,
      typename T::element_type* ptr)
    {
        if (ptr) {
            fromType<T::element_type>(writer, ptr);
        }
    }

    template<class T>
    static void convertToJsonArray(minijson::object_writer& writer,
      const T& container,
      const char* key)
    {
        minijson::array_writer nested_writer = writer.nested_array(key);
        for (const auto& i : container) {
            fromTypeArray<typename T::value_type>(nested_writer, i);
        }
        nested_writer.close();
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const std::vector<typename T::value_type>& inputVector,
      const char* key)
    {
        convertToJsonArray(writer, inputVector, key);
    }

    template<class T>
    static void fromType(minijson::object_writer& writer,
      const std::set<typename T::value_type>& inputSet,
      const char* key)
    {
        convertToJsonArray(writer, inputSet, key);
    }
};
} // namespace Cerializer
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
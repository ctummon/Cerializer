#pragma once

#include <simdjson.h>

#include <optional>
#include <set>
#include <type_traits>
#include <vector>

#include "CerialUtils.h"

namespace Cerializer {
template<typename>
class SimdJsonObj;

struct SimdJsonConverter
{
    // using JsonObj = simdjson::ParsedJson;
    using JsonVal =
      simdjson::simdjson_result<simdjson::fallback::ondemand::value>;

    static const auto getField(simdjson::ondemand::document& doc,
      const char* fieldName)
    {
        return doc[fieldName];
    }

    // De-serialize
    template<class T,
      typename std::enable_if<
        std::is_same<std::optional<typename T::value_type>, T>::value>::type* =
        nullptr>
    T toType(const JsonVal& val)
    {
        return std::make_optional<T::value_type>(toType<T::value_type>(v));
    }

    template<class T,
      typename std::enable_if<
        std::is_base_of<Cerializer::SimdJsonObj<T>, T>::value>::type* = nullptr>
    static T toType(const JsonVal& val)
    {
        return T{}; // T::fromJson(val.get_object());
    }

    template<class T,
      typename std::enable_if<std::is_same<short, T>::value>::type* = nullptr>
    static T toType(const JsonVal& v)
    {
        short returnVal{};
        /*if (v.IsInt())
    {
        returnVal = static_cast<short>(v.GetInt());
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned short, T>::value>::type* =
        nullptr>
    static T toType(const JsonVal& v)
    {
        unsigned short returnVal{};
        /*if (v.IsUint())
    {
        returnVal = static_cast<unsigned short>(v.GetUint());
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<int, T>::value>::type* = nullptr>
    static T toType(const JsonVal& v)
    {
        int returnVal{};
        /*if (v.IsInt())
    {
        returnVal = v.GetInt();
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned int, T>::value>::type* =
        nullptr>
    static T toType(const JsonVal& v)
    {
        unsigned int returnVal{};
        /*if (v.IsUint())
    {
        returnVal = v.GetUint();
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<long, T>::value>::type* = nullptr>
    static T toType(const JsonVal& v)
    {
        long returnVal{};
        /*if (v.IsInt64())
    {
        returnVal = static_cast<long>(v.GetInt64());
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned long, T>::value>::type* =
        nullptr>
    static T toType(const JsonVal& v)
    {
        unsigned long returnVal{};
        /*if (v.IsUint64())
    {
        returnVal = static_cast<unsigned long>(v.GetUint64());
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<long long, T>::value>::type* =
        nullptr>
    static T toType(const JsonVal& v)
    {
        long long returnVal{};
        /*if (v.IsInt64())
    {
        returnVal = static_cast<long long>(v.GetInt64());
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::wstring, T>::value>::type* =
        nullptr>
    static T toType(const JsonVal& v)
    {
        T returnVal;
        /*if (v.IsString())
    {
        returnVal = Utils::stringConverter.from_bytes(v.GetString());
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::string, T>::value>::type* =
        nullptr>
    static T toType(const JsonVal& v)
    {
        T returnVal;
        /*if (v.IsString())
    {
        returnVal = v.GetString();
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<double, T>::value>::type* = nullptr>
    static T toType(const JsonVal& v)
    {
        double returnVal{};
        /*if (v.IsNumber())
    {
        returnVal = v.GetDouble();
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<float, T>::value>::type* = nullptr>
    static T toType(const JsonVal& v)
    {
        float returnVal{};
        /*if (v.IsNumber())
    {
        returnVal = static_cast<float>(v.GetDouble());
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<bool, T>::value>::type* = nullptr>
    static T toType(const JsonVal& v)
    {
        bool returnVal{};
        /*if (v.IsBool())
    {
        returnVal = v.GetBool();
    }*/
        return returnVal;
    }

    template<class T,
      typename std::enable_if<
        std::is_same<std::shared_ptr<typename T::element_type>,
          T>::value>::type* = nullptr>
    static T toType(const JsonVal& v)
    {
        return std::make_shared<T::element_type>(toType<T::element_type>(v));
    }

    template<class T,
      typename std::enable_if<std::is_same<std::vector<typename T::value_type>,
        T>::value>::type* = nullptr>
    static T toType(const JsonVal& v)
    {
        T returnArray;
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
    static T toType(const JsonVal& v)
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
    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      Cerializer::SimdJsonObj<T>& val)
    {
        // val.appendJsonStr(writer);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const Cerializer::SimdJsonObj<T>& val)
    {
        // val.appendJsonStr(writer);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc, const short val)
    {
        // writer.Int(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const unsigned short val)
    {
        // writer.Uint(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc, const int val)
    {
        // writer.Int(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const unsigned int val)
    {
        // writer.Uint(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc, const long val)
    {
        // writer.Int64(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const unsigned long val)
    {
        // writer.Uint(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc, const long long val)
    {
        // writer.Int64(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc, const double val)
    {
        // writer.Double(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc, const float val)
    {
        // writer.Double(static_cast<double>(val));
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc, const bool val)
    {
        // writer.Bool(val);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const std::wstring& val)
    {
        // writer.String(Utils::stringConverter.to_bytes(val).c_str());
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const std::string& val)
    {
        // writer.String(val.c_str());
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const std::shared_ptr<typename T::element_type>& ptr)
    {
        if (ptr) {
            fromType<T::element_type>(doc, *ptr);
        }
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      typename T::element_type* ptr)
    {
        if (ptr) {
            fromType<T::element_type>(doc, ptr);
        }
    }

    template<class T>
    static void convertToJsonArray(simdjson::ondemand::document& doc,
      const T& container)
    {
        for (const auto& i : container) {
            // move key writing to main loop.
            fromType<typename T::value_type>(doc, i);
        }
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const std::vector<typename T::value_type>& inputVector)
    {
        convertToJsonArray(doc, inputVector);
    }

    template<class T>
    static void fromType(simdjson::ondemand::document& doc,
      const std::set<typename T::value_type>& inputSet)
    {
        convertToJsonArray(doc, inputSet);
    }
};
} // namespace Cerializer
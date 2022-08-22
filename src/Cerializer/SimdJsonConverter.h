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
        //TODO: Refactor how SimdJsonConverter and SimdJsonObj interact.
        assert("Not supported");
        return T{};
    }

    template<class T,
      typename std::enable_if<std::is_same<short, T>::value>::type* = nullptr>
    static T toType(JsonVal v)
    {
        short returnVal{};

        auto ir = v.get_int64();
        if(!ir.error()){
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
        if(!ir.error()){
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
        if(!ir.error()){
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
        if(!ir.error()){
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
        if(!ir.error()){
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
        if(!ir.error()){
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
        if(!ir.error()){
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
        if(!sr.error()){
            auto stringView = sr.value_unsafe();
            //todo: fix
            returnVal = Utils::stringConverter.from_bytes(&*stringView.begin(), &*stringView.begin());
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
        if(!sr.error()){
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
        if(!dr.error()){
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
        if(!fr.error()){
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
        if(!br.error()){
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
        //Should be handled elsewhere, need to refactor
        assert("Not supported");
        return std::make_shared<T::element_type>();
    }

    template<class T,
      typename std::enable_if<std::is_same<std::vector<typename T::value_type>,
        T>::value>::type* = nullptr>
    static T toType(JsonVal v)
    {
        T returnArray;
        //need to account for array of objects. Whole class needs a refactor.
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
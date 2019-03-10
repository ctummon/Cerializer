#pragma once

#include "rapidjson/document.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


#include <set>
#include <vector>

#include <type_traits>

#include <locale>
#include <codecvt>
#include <string>

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

namespace Cerial
{
    template<typename> class RapidJsonObj;
}

struct RapidJsonConverter
{
    static bool fieldExists(const rapidjson::Document& data, const char* fieldName)
    {
        return data.HasMember(fieldName);
    }

    static rapidjson::Value::ConstMemberIterator getField(const rapidjson::Document& data, const char* fieldName)
    {
        return data.FindMember(fieldName);
    }

    //De-serialize 
    template<class T, typename std::enable_if<std::is_base_of<Cerial::RapidJsonObj<T>, T >::value>::type* = nullptr>
    static T toType(const rapidjson::Document& data)
    {
        auto& v = (*data).value;
        if (v.IsObject())
        {
            return T::fromJson(v.GetObjectA());
        }
        return T();
    }

    template<class T, typename std::enable_if<std::is_same<unsigned short, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        unsigned short returnVal{ 0 };
        /*if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_int32())
            {
                returnVal = num.to_int32();
            }
        }*/
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<int, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        int returnVal{ 0 };
        /*if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_int32())
            {
                returnVal = num.to_int32();
            }
        }*/
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<unsigned int, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        unsigned int returnVal{ 0 };
        /*if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_uint32())
            {
                returnVal = num.to_uint32();
            }
        }*/
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<long, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        long returnVal{ 0 };
        /*if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_int64())
            {
                returnVal = num.to_int64();
            }
        }*/
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<unsigned long, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        unsigned long returnVal{ 0 };
        /*if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_uint64())
            {
                returnVal = num.to_uint64();
            }
        }*/
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<long long, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        long long returnVal{ 0 };
        /*if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_uint64())
            {
                returnVal = num.to_uint64();
            }
        }*/
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<std::wstring, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        /*if (!data.is_string())
        {
            return T();
        }
#ifdef _WIN32
        return data.as_string();
#else
        return StringUtils::toWString(data.as_string());
#endif*/
        return T();
    }

    template<class T, typename std::enable_if<std::is_same<std::string, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        /*if (!data.is_string())
        {
            return T();
        }
        return StringUtils::fromSparkString(data.as_string());*/
        return T();
    }

    template<class T, typename std::enable_if<std::is_same<double, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        /*if (!data.is_double())
        {
            return 0;
        }
        return data.as_double();*/
        return 0;
    }

    template<class T, typename std::enable_if<std::is_same<bool, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        /*
        if (!data.is_boolean())
        {
            return false;
        }
        return data.as_bool();*/
        return false;
    }

    template<class T, typename std::enable_if<std::is_same<std::shared_ptr<typename T::element_type>, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        return std::make_shared<T::element_type>(toType<T::element_type>(data));
    }

    template<class T, typename std::enable_if<std::is_same<std::vector<typename T::value_type>, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        T returnArray;
        /*if (data.is_array())
        {
            const auto& arrayData = data.as_array();
            for (const auto& i : arrayData)
            {
                returnArray.push_back(toType<typename T::value_type>(i));
            }
        }*/
        return returnArray;
    }

    template<class T, typename std::enable_if<std::is_same<std::set<typename T::value_type>, T>::value>::type* = nullptr>
    static T toType(const rapidjson::Value::ConstMemberIterator& data)
    {
        T returnArray;
        /*if (data.is_array())
        {
            const auto& arrayData = data.as_array();
            for (const auto& i : arrayData)
            {
                returnArray.insert(toType<typename T::value_type>(i));
            }
        }*/
        return returnArray;
    }

    //Serialize
    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, Cerial::RapidJsonObj<T>& val)
    {
        writer.Key(fieldName);
        val.appendJsonStr(writer);
    }

    template<class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const Cerial::RapidJsonObj<T>& val)
    {
        writer.Key(fieldName);
        val.appendJsonStr(writer);
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const int& val)
    {
        writer.Key(fieldName);
        writer.Int(val);
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const unsigned int& val)
    {
        writer.Key(fieldName);
        writer.Uint(val);
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const long& val)
    {
        writer.Key(fieldName);
        writer.Int64(val);
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const unsigned long& val)
    {
        writer.Key(fieldName);
        writer.Uint(val);
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const long long& val)
    {
        writer.Key(fieldName);
        writer.Int64(val);
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const double& val)
    {
        writer.Key(fieldName);
        writer.Double(val);
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const bool& val)
    {
        writer.Key(fieldName);
        writer.Bool(val);
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::wstring& val)
    {
        writer.Key(fieldName);
        writer.String(converter.from_bytes(val.as_string()));
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& val)
    {
        //return web::json::value(StringUtils::toSparkString(val));
        //return rapidjson::Document();
    }

    template <class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::shared_ptr<typename T::element_type>& ptr)
    {
        if (ptr)
        {
            fromType<T::element_type>(*ptr);
        }
    }

    template<class T>
    static void convertToJsonArray(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const T& container)
    {
        /*std::vector<web::json::value> jsonValArray;

        for (const auto& i : container)
        {
            jsonValArray.push_back(fromType<typename T::value_type>(i));
        }

        return web::json::value::array(jsonValArray);*/
    }

    template<class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::vector<typename T::value_type>& inputVector)
    {
        convertToJsonArray(fieldName, writer, inputVector);
    }

    template<class T>
    static void fromType(const char* fieldName, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::set<typename T::value_type>& inputSet)
    {
        convertToJsonArray(fieldName, writer, inputSet);
    }
};
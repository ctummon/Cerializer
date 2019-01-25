#pragma once

#include <cpprest/json.h>

#include <set>

#include <type_traits>

#include <locale>
#include <codecvt>
#include <string>

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

namespace Cerial
{
    template<typename> class CppRestJsonObj;
}

struct CppRestJsonConverter
{
    using JsonObj = web::json::value;
        
    template<typename IdentifierType>
    static bool fieldExists(const JsonObj& data, const IdentifierType* fieldName)
    {
        return data.has_field(fieldName);
    }

    template<typename IdentifierType>
    static JsonObj getField(const JsonObj& data, const IdentifierType* fieldName)
    {
        return data.at(fieldName);
    }

    template<typename IdentifierType>
    static void populateJsonObj(const IdentifierType* fieldName, JsonObj& data, const JsonObj& memberVar)
    {
        data[fieldName] = memberVar;
    }

    template<class T, typename std::enable_if<std::is_base_of<Cerial::CppRestJsonObj<T>, T >::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        return T::fromJson(data);
    }

    template<class T, typename std::enable_if<std::is_same<unsigned short, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        unsigned short returnVal{ 0 };
        if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_int32())
            {
                returnVal = static_cast<unsigned short>(num.to_int32());
            }
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<int, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        int returnVal{ 0 };
        if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_int32())
            {
                returnVal = num.to_int32();
            }
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<unsigned int, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        unsigned int returnVal{ 0 };
        if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_uint32())
            {
                returnVal = num.to_uint32();
            }
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<long, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        long returnVal{ 0 };
        if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_int64())
            {
                returnVal = static_cast<long>(num.to_int64());
            }
        }
        return returnVal;
    }
  
    template<class T, typename std::enable_if<std::is_same<unsigned long, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        unsigned long returnVal{ 0 };
        if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_uint64())
            {
                returnVal = static_cast<unsigned long>(num.to_uint64());
            }
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<long long, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        long long returnVal{ 0 };
        if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_int64())
            {
                returnVal = num.to_int64();
            }
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<unsigned long long, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        uint64_t returnVal{ 0 };
        if (data.is_number())
        {
            const auto& num = data.as_number();
            if (num.is_uint64())
            {
                returnVal = num.to_uint64();
            }
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<std::wstring, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        if (!data.is_string())
        {
            return T();
        }
#ifdef _WIN32
        return data.as_string();
#else
        return converter.from_bytes(data.as_string());
#endif
    }



    template<class T, typename std::enable_if<std::is_same<std::string, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        if (!data.is_string())
        {
            return T();
        }
#ifdef _WIN32
        return converter.to_bytes(data.as_string());
#else
        return data.as_string();
#endif
    }

    template<class T, typename std::enable_if<std::is_same<double, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        if (!data.is_double())
        {
            return 0;
        }
        return data.as_double();
    }

    template<class T, typename std::enable_if<std::is_same<float, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        if (!data.is_double())
        {
            return 0;
        }
        return static_cast<float>(data.as_double());
    }

    template<class T, typename std::enable_if<std::is_same<bool, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        if (!data.is_boolean())
        {
            return false;
        }
        return data.as_bool();
    }

    template<class T, typename std::enable_if<std::is_same<std::shared_ptr<typename T::element_type>, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        return std::make_shared<T::element_type>(toType<T::element_type>(data));
    }

    template<class T, typename std::enable_if<std::is_same<std::vector<typename T::value_type>, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        T returnArray;
        if (data.is_array())
        {
            const auto& arrayData = data.as_array();
            for (const auto& i : arrayData)
            {
                returnArray.push_back(toType<typename T::value_type>(i));
            }
        }
        return returnArray;
    }

    template<class T, typename std::enable_if<std::is_same<std::set<typename T::value_type>, T>::value>::type* = nullptr>
    static T toType(const web::json::value& data)
    {
        T returnArray;
        if (data.is_array())
        {
            const auto& arrayData = data.as_array();
            for (const auto& i : arrayData)
            {
                returnArray.insert(toType<typename T::value_type>(i));
            }
        }
        return returnArray;
    }

    template <class T>
    static web::json::value fromType(Cerial::CppRestJsonObj<T>& val)
    {
        return val.toJson();
    }

    template<class T>
    static web::json::value fromType(const Cerial::CppRestJsonObj<T>& val)
    {
        return val.toJson();
    }

    template <class T>
    static web::json::value fromType(const unsigned short& val)
    {
        return web::json::value::number(static_cast<int>(val));
    }

    template <class T>
    static web::json::value fromType(const int& val)
    {
        return web::json::value::number(val);
    }

    template <class T>
    static web::json::value fromType(const unsigned int& val)
    {
        return web::json::value::number(val);
    }

    template <class T>
    static web::json::value fromType(const long& val)
    {
        return web::json::value::number(val);
    }

    template <class T>
    static web::json::value fromType(const unsigned long& val)
    {
        return web::json::value::number(static_cast<uint64_t>(val));
    }

    template <class T>
    static web::json::value fromType(const long long& val)
    {
        return web::json::value::number(val);
    }

    template <class T>
    static web::json::value fromType(const unsigned long long& val)
    {
        return web::json::value::number(val);
    }

    template <class T>
    static web::json::value fromType(const double& val)
    {
        return web::json::value::number(val);
    }

    template <class T>
    static web::json::value fromType(const float& val)
    {
        return web::json::value::number(static_cast<double>(val));
    }

    template <class T>
    static web::json::value fromType(const bool& val)
    {
        return web::json::value(val);
    }

    template <class T>
    static web::json::value fromType(const std::wstring& val)
    {

#ifdef _WIN32
        return web::json::value(val);
#else
        return web::json::value(converter.to_bytes(val));
#endif
    }

    template <class T>
    static web::json::value fromType(const std::string& val)
    {
#ifdef _WIN32
        return web::json::value(converter.from_bytes(val));
#else
        return web::json::value(val);   
#endif
    }

    template <class T>
    static web::json::value fromType(const std::shared_ptr<typename T::element_type>& ptr)
    {
        if (ptr)
        {
            return fromType<T::element_type>(*ptr);
        }

        return web::json::value();
    }

    template<class T>
    static web::json::value convertToJsonArray(const T& container)
    {
        std::vector<web::json::value> jsonValArray;

        for (const auto& i : container)
        {
            jsonValArray.push_back(fromType<typename T::value_type>(i));
        }

        return web::json::value::array(jsonValArray);
    }

    template<class T>
    static web::json::value fromType(const std::vector<typename T::value_type>& inputVector)
    {
        return convertToJsonArray(inputVector);
    }

    template<class T>
    static web::json::value fromType(const std::set<typename T::value_type>& inputSet)
    {
        return convertToJsonArray(inputSet);
    }
};

#pragma once

#include "CerialUtils.h"

#include <nlohmann/json.hpp>

#include <set>
#include <vector>

#include <type_traits>

namespace Cerial
{
    template<typename> class NlohmannJsonObj;
}

struct NlohmannJsonConverter
{
    using JsonObj = nlohmann::json;

    static auto endItr(const JsonObj& data)
    {
        return data.end();
    }

    static auto getField(const JsonObj& data, const char* fieldName)
    {
        return data.find(fieldName);
    }

    static auto getValue(const nlohmann::json::const_iterator& itr)
    {
        return *itr;
    }

    static void populateJsonObj(const char* fieldName, JsonObj& data, const JsonObj::value_type& memberVar)
    {
        data[fieldName] = memberVar;
    }

    template<class T, typename std::enable_if<std::is_base_of<Cerial::NlohmannJsonObj<T>, T >::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        if (data.is_object())
        {
            return T::fromJson(data);
        }
        return T();
    }

    template<class T, typename std::enable_if<std::is_same<short, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        short returnVal{};
        if (data.is_number_integer())
        {
            returnVal = data.get<short>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<unsigned short, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        unsigned short returnVal{ 0 };
        if (data.is_number_unsigned())
        {
            returnVal = data.get<unsigned short>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<int, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        int returnVal{ 0 };
        if (data.is_number_integer())
        {
            returnVal = data.get<int>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<unsigned int, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        unsigned int returnVal{ 0 };
        if (data.is_number_unsigned())
        {
            returnVal = data.get<unsigned int>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<long, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        long returnVal{ 0 };
        if (data.is_number_integer())
        {
            returnVal = data.get<long>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<unsigned long, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        unsigned long returnVal{ 0 };
        if (data.is_number_unsigned())
        {
            returnVal = data.get<unsigned long>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<long long, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        long long returnVal{ 0 };
        if (data.is_number_integer())
        {
            returnVal = data.get<long long>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<std::wstring, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        std::wstring returnVal;
        if (data.is_string())
        {
            returnVal = stringConverter.from_bytes(data.get<std::string>());
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<std::string, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        std::string returnVal;
        if (data.is_string())
        {
            returnVal = data.get<std::string>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<double, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        double returnVal{ 0 };
        if (data.is_number_float())
        {
            returnVal = data.get<double>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<float, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        float returnVal{ 0 };
        if (data.is_number_float())
        {
            returnVal = data.get<float>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<bool, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        bool returnVal{ false };
        if (data.is_boolean())
        {
            returnVal = data.get<bool>();
        }
        return returnVal;
    }

    template<class T, typename std::enable_if<std::is_same<std::shared_ptr<typename T::element_type>, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        return std::make_shared<T::element_type>(toType<T::element_type>(data));
    }

    template<class T, typename std::enable_if<std::is_same<std::vector<typename T::value_type>, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        T returnArray;
        if (data.is_array())
        {
            for (const auto& i : data)
            {
                returnArray.push_back(toType<typename T::value_type>(i));
            }
        }
        return returnArray;
    }

    template<class T, typename std::enable_if<std::is_same<std::set<typename T::value_type>, T>::value>::type* = nullptr>
    static T toType(const JsonObj::value_type& data)
    {
        T returnSet;
        if (data.is_array())
        {
            for (const auto& i : data)
            {
                returnSet.insert(toType<typename T::value_type>(i));
            }
        }
        return returnSet;
    }

    template <class T>
    static JsonObj::value_type fromType(Cerial::NlohmannJsonObj<T>& val)
    {
        return val.toJson();
    }

    template<class T>
    static JsonObj::value_type fromType(const Cerial::NlohmannJsonObj<T>& val)
    {
        return val.toJson();
    }

    template <class T>
    static JsonObj::value_type fromType(const short& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const unsigned short& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const int& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const unsigned int& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const long& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const unsigned long& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const long long& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const double& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const bool& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const std::wstring& val)
    {
        return JsonObj::value_type(stringConverter.to_bytes(val));
    }

    template <class T>
    static JsonObj::value_type fromType(const std::string& val)
    {
        return JsonObj::value_type(val);
    }

    template <class T>
    static JsonObj::value_type fromType(const std::shared_ptr<typename T::element_type>& ptr)
    {
        if (ptr)
        {
            return fromType<T::element_type>(*ptr);
        }
        return JsonObj::value_type();
    }

    template<class T>
    static JsonObj::value_type convertToJsonArray(const T& container)
    {
        JsonObj::array_t jsonValArray;

        for (const auto& i : container)
        {
            jsonValArray.push_back(fromType<typename T::value_type>(i));
        }

        return JsonObj::value_type(jsonValArray);
    }

    template<class T>
    static JsonObj::value_type fromType(const std::vector<typename T::value_type>& inputVector)
    {
        return convertToJsonArray(inputVector);
    }

    template<class T>
    static JsonObj::value_type fromType(const std::set<typename T::value_type>& inputSet)
    {
        return convertToJsonArray(inputSet);
    }
};
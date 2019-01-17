#pragma once

#include "rapidjson/document.h"
//#include "rapidjson/rapidjson.h"

#include <set>

#include <type_traits>

namespace Serializable
{
    template<typename> class Object;
}

namespace ConverterUtils
{
    namespace JsonConverter
    {
        //De-serialize 
        template<class T, typename std::enable_if<std::is_base_of<Serializable::Object<T>, T >::value>::type* = nullptr>
        T toType(const rapidjson::Document& data)
        {
            return T::fromJson(data);
        }

        template<class T, typename std::enable_if<std::is_same<int32_t, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
        {
            int32_t returnVal{ 0 };
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

        template<class T, typename std::enable_if<std::is_same<uint32_t, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
        {
            uint32_t returnVal{ 0 };
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

        template<class T, typename std::enable_if<std::is_same<int64_t, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
        {
            int64_t returnVal{ 0 };
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

        template<class T, typename std::enable_if<std::is_same<uint64_t, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
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
        T toType(const web::json::value& data)
        {
            if (!data.is_string())
            {
                return T();
            }
#ifdef _WIN32
            return data.as_string();
#else
            return StringUtils::toWString(data.as_string());
#endif
        }

        template<class T, typename std::enable_if<std::is_same<std::string, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
        {
            if (!data.is_string())
            {
                return T();
            }
            return StringUtils::fromSparkString(data.as_string());
        }

        template<class T, typename std::enable_if<std::is_same<double, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
        {
            if (!data.is_double())
            {
                return 0;
            }
            return data.as_double();
        }

        template<class T, typename std::enable_if<std::is_same<bool, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
        {
            if (!data.is_boolean())
            {
                return false;
            }
            return data.as_bool();
        }

        template<class T, typename std::enable_if<std::is_same<std::shared_ptr<typename T::element_type>, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
        {
            return std::make_shared<T::element_type>(toType<T::element_type>(data));
        }

        template<class T, typename std::enable_if<std::is_same<std::vector<typename T::value_type>, T>::value>::type* = nullptr>
        T toType(const web::json::value& data)
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
        T toType(const web::json::value& data)
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

        //Serialize
        template <class T>
        web::json::value fromType(Serializable::Object<T>& val)
        {
            return val.toJson();
        }

        template<class T>
        web::json::value fromType(const Serializable::Object<T>& val)
        {
            return val.toJson();
        }

        template <class T>
        web::json::value fromType(const int32_t& val)
        {
            return web::json::value(val);
        }

        template <class T>
        web::json::value fromType(const uint32_t& val)
        {
            return web::json::value(val);
        }

        template <class T>
        web::json::value fromType(const int64_t& val)
        {
            return web::json::value(val);
        }

        template <class T>
        web::json::value fromType(const uint64_t& val)
        {
            return web::json::value(val);
        }

        template <class T>
        web::json::value fromType(const double& val)
        {
            return web::json::value(val);
        }

        template <class T>
        web::json::value fromType(const bool& val)
        {
            return web::json::value(val);
        }

        template <class T>
        web::json::value fromType(const std::wstring& val)
        {
#ifdef _WIN32
            return web::json::value(val);
#else
            return web::json::value(StringUtils::toString(val));
#endif
        }

        template <class T>
        web::json::value fromType(const std::string& val)
        {
            return web::json::value(StringUtils::toSparkString(val));
        }

        template <class T>
        web::json::value fromType(const std::shared_ptr<typename T::element_type>& ptr)
        {
            if (ptr)
            {
                return fromType<T::element_type>(*ptr);
            }

            return web::json::value();
        }

        template<class T>
        web::json::value convertToJsonArray(const T& container)
        {
            std::vector<web::json::value> jsonValArray;

            for (const auto& i : container)
            {
                jsonValArray.push_back(fromType<typename T::value_type>(i));
            }

            return web::json::value::array(jsonValArray);
        }

        template<class T>
        web::json::value fromType(const std::vector<typename T::value_type>& inputVector)
        {
            return convertToJsonArray(inputVector);
        }

        template<class T>
        web::json::value fromType(const std::set<typename T::value_type>& inputSet)
        {
            return convertToJsonArray(inputSet);
        }
    }
}

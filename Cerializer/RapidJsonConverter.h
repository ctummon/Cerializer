#pragma once

#include "rapidjson/document.h"


#include <set>
#include <vector>

#include <type_traits>

namespace Cerial
{
    template<typename> class RapidJsonObj;
}

    struct RapidJsonConverter
    {
        using JsonObj = rapidjson::Value;

        static bool fieldExists(const JsonObj& data, const char* fieldName)
        {
            data.HasMember(fieldName);
        }

        static rapidjson::Value::ConstMemberIterator getField(const JsonObj& data, const char* fieldName)
        {
            return data.FindMember(fieldName);
        }

        static void populateJsonObj(const char* fieldName, JsonObj& data, JsonObj& memberVar)
        {
            //rapidjson::Document d;
            //auto w = rapidjson::Value(fieldName);
            //data.AddMember(fieldName, memberVar, d.GetAllocator());
            //data.AddMember(rapidjson::Value(fieldName, memberVar),d.GetAllocator());
        }

        //De-serialize 
        template<class T, typename std::enable_if<std::is_base_of<Cerial::RapidJsonObj<T>, T >::value>::type* = nullptr>
        static T toType(const JsonObj& data)
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
        static rapidjson::Document fromType(Cerial::RapidJsonObj<T>& val)
        {
            return val.toJson();
        }

        template<class T>
        static rapidjson::Document fromType(const Cerial::RapidJsonObj<T>& val)
        {
            return val.toJson();
        }

        template <class T>
        static rapidjson::Document fromType(const int& val)
        {
            //return web::json::value(val);
            return rapidjson::Document();
        }

        template <class T>
        static rapidjson::Document fromType(const unsigned int& val)
        {
            //return web::json::value(val);
            return rapidjson::Document();
        }

        template <class T>
        static rapidjson::Document fromType(const long& val)
        {
            //return web::json::value(val);
            return rapidjson::Document();
        }

        template <class T>
        static rapidjson::Document fromType(const unsigned long& val)
        {
            return rapidjson::Document();
            //return web::json::value(val);
        }

        template <class T>
        static rapidjson::Document fromType(const long long& val)
        {
            return rapidjson::Document();
            //return web::json::value(val);
        }

        template <class T>
        static rapidjson::Document fromType(const double& val)
        {
            return rapidjson::Document();
            //return web::json::value(val);
        }

        template <class T>
        static rapidjson::Document fromType(const bool& val)
        {
            return rapidjson::Document();
            //return web::json::value(val);
        }

        template <class T>
        static rapidjson::Document fromType(const std::wstring& val)
        {
/*#ifdef _WIN32
            return web::json::value(val);
#else
            return web::json::value(StringUtils::toString(val));
#endif*/
            return rapidjson::Document();
        }

        template <class T>
        static rapidjson::Document fromType(const std::string& val)
        {
            //return web::json::value(StringUtils::toSparkString(val));
            return rapidjson::Document();
        }

        template <class T>
        static rapidjson::Document fromType(const std::shared_ptr<typename T::element_type>& ptr)
        {
            if (ptr)
            {
                return fromType<T::element_type>(*ptr);
            }

            return rapidjson::Document();
            //return web::json::value();
        }

        template<class T>
        static rapidjson::Document convertToJsonArray(const T& container)
        {
            /*std::vector<web::json::value> jsonValArray;

            for (const auto& i : container)
            {
                jsonValArray.push_back(fromType<typename T::value_type>(i));
            }

            return web::json::value::array(jsonValArray);*/
            return rapidjson::Document();
        }

        template<class T>
        static rapidjson::Document fromType(const std::vector<typename T::value_type>& inputVector)
        {
            return convertToJsonArray(inputVector);
        }

        template<class T>
        static rapidjson::Document fromType(const std::set<typename T::value_type>& inputSet)
        {
            return convertToJsonArray(inputSet);
        }
    };
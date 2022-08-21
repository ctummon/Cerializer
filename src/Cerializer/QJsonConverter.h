#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <optional>
#include <set>
#include <type_traits>
#include <vector>

namespace Cerializer {
template<typename>
class QJsonObj;
struct QJsonConverter
{
    using JsonObj = QJsonObject;

    static auto endItr(const JsonObj& data) { return data.end(); }

    static auto getField(const JsonObj& data, const char* fieldName)
    {
        return data.find(fieldName);
    }

    static auto getValue(const QJsonObject::const_iterator& itr)
    {
        return itr.value();
    }

    static void populateJsonObj(const char* fieldName,
      JsonObj& data,
      const QJsonValue& memberVar)
    {
        data.insert(fieldName, memberVar);
    }

    // De-serialize
    template<class T,
      typename std::enable_if<
        std::is_same<std::optional<typename T::value_type>, T>::value>::type* =
        nullptr>
    static T toType(const QJsonValue& data)
    {
        return std::make_optional<T::value_type>(toType<T::value_type>(data));
    }

    template<class T,
      typename std::enable_if<
        std::is_base_of<Cerializer::QJsonObj<T>, T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        if (data.isObject()) {
            return T::fromJson(data.toObject());
        }
        return T();
    }

    template<class T,
      typename std::enable_if<std::is_same<short, T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        short returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = static_cast<short>(data.toDouble());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned short, T>::value>::type* =
        nullptr>
    static T toType(const QJsonValue& data)
    {
        unsigned short returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = static_cast<unsigned short>(data.toDouble());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<int, T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        int returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = static_cast<int>(data.toDouble());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned int, T>::value>::type* =
        nullptr>
    static T toType(const QJsonValue& data)
    {
        unsigned int returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = static_cast<unsigned int>(data.toDouble());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<long, T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        long returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = static_cast<long>(data.toDouble());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<unsigned long, T>::value>::type* =
        nullptr>
    static T toType(const QJsonValue& data)
    {
        unsigned long returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = static_cast<unsigned long>(data.toDouble());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<long long, T>::value>::type* =
        nullptr>
    static T toType(const QJsonValue& data)
    {
        long long returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = static_cast<long long>(data.toDouble());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::wstring, T>::value>::type* =
        nullptr>
    static T toType(const QJsonValue& data)
    {
        std::wstring returnVal;
        if (data.isString()) {
            returnVal = data.toString().toStdWString();
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::string, T>::value>::type* =
        nullptr>
    static T toType(const QJsonValue& data)
    {
        std::string returnVal;
        if (data.isString()) {
            returnVal = data.toString().toStdString();
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<QString, T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        QString returnVal;
        if (data.isString()) {
            returnVal = data.toString();
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<double, T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        double returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = data.toDouble();
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<float, T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        float returnVal{ 0 };
        if (data.isDouble()) {
            returnVal = static_cast<float>(data.toDouble());
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<std::is_same<bool, T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        bool returnVal{ false };
        if (data.isBool()) {
            returnVal = data.toBool();
        }
        return returnVal;
    }

    template<class T,
      typename std::enable_if<
        std::is_same<std::shared_ptr<typename T::element_type>,
          T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        return std::make_shared<T::element_type>(toType<T::element_type>(data));
    }

    template<class T,
      typename std::enable_if<std::is_same<std::vector<typename T::value_type>,
        T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        T returnArray;
        if (data.isArray()) {
            const auto& arrayData = data.toArray();
            for (const auto& i : arrayData) {
                returnArray.push_back(toType<typename T::value_type>(i));
            }
        }
        return returnArray;
    }

    template<class T,
      typename std::enable_if<std::is_same<std::set<typename T::value_type>,
        T>::value>::type* = nullptr>
    static T toType(const QJsonValue& data)
    {
        T returnSet;
        if (data.isArray()) {
            const auto& arrayData = data.toArray();
            for (const auto& i : arrayData) {
                returnSet.insert(toType<typename T::value_type>(i));
            }
        }
        return returnSet;
    }

    // Serialize
    template<class T>
    static QJsonValue fromType(Cerializer::QJsonObj<T>& val)
    {
        return val.toJson();
    }

    template<class T>
    static QJsonValue fromType(const Cerializer::QJsonObj<T>& val)
    {
        return val.toJson();
    }

    template<class T>
    static QJsonValue fromType(const short& val)
    {
        return QJsonValue(static_cast<int>(val));
    }

    template<class T>
    static QJsonValue fromType(const unsigned short& val)
    {
        return QJsonValue(static_cast<int>(val));
    }

    template<class T>
    static QJsonValue fromType(const int& val)
    {
        return QJsonValue(val);
    }

    template<class T>
    static QJsonValue fromType(const unsigned int& val)
    {
        return QJsonValue(static_cast<qint64>(val));
    }

    template<class T>
    static QJsonValue fromType(const long& val)
    {
        return QJsonValue(static_cast<qint64>(val));
    }

    template<class T>
    static QJsonValue fromType(const unsigned long& val)
    {
        return QJsonValue(static_cast<qint64>(val));
    }

    template<class T>
    static QJsonValue fromType(const long long& val)
    {
        return QJsonValue(static_cast<qint64>(val));
    }

    template<class T>
    static QJsonValue fromType(const double& val)
    {
        return QJsonValue(val);
    }

    template<class T>
    static QJsonValue fromType(const float& val)
    {
        return QJsonValue(static_cast<double>(val));
    }

    template<class T>
    static QJsonValue fromType(const bool& val)
    {
        return QJsonValue(val);
    }

    template<class T>
    static QJsonValue fromType(const std::wstring& val)
    {
        return QJsonValue(QString::fromStdWString(val));
    }

    template<class T>
    static QJsonValue fromType(const std::string& val)
    {
        return QJsonValue(QString::fromStdString(val));
    }

    template<class T>
    static QJsonValue fromType(const QString& val)
    {
        return QJsonValue(val);
    }

    template<class T>
    static QJsonValue fromType(
      const std::shared_ptr<typename T::element_type>& ptr)
    {
        if (ptr) {
            return fromType<T::element_type>(*ptr);
        }
        return QJsonValue();
    }

    template<class T>
    static QJsonValue convertToJsonArray(const T& container)
    {
        QJsonArray jsonValArray;

        for (const auto& i : container) {
            jsonValArray.push_back(fromType<typename T::value_type>(i));
        }

        return QJsonValue(jsonValArray);
    }

    template<class T>
    static QJsonValue fromType(
      const std::vector<typename T::value_type>& inputVector)
    {
        return convertToJsonArray(inputVector);
    }

    template<class T>
    static QJsonValue fromType(const std::set<typename T::value_type>& inputSet)
    {
        return convertToJsonArray(inputSet);
    }
};
} // namespace Cerializer
#pragma once

#include "Cerializer/Serializable.h"
#include "Cerializer/QJsonConverter.h"

namespace Cerializer {
    template <typename T>
    class QJsonObj : public Serializable::Object<T, QJsonConverter> {
    public:
        virtual ~QJsonObj<T>() = default;
    private:
        QJsonObj() = default;
        friend T;
    };
}
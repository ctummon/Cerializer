#pragma once

#include "Cerializer/Serializable.h"
#include "Cerializer/CppRestJsonConverter.h"

namespace Cerializer {
    template <typename T>
    class CppRestJsonObj : public Serializable::Object<T, CppRestJsonConverter> {
    public:
        virtual ~CppRestJsonObj<T>() = default;
    private:
        CppRestJsonObj() = default;
        friend T;
    };
}
#pragma once

#include "Serializable.h"
#include "NlohmannJsonConverter.h"

namespace Cerializer {
    template <typename T>
    class NlohmannJsonObj : public Serializable::Object<T, NlohmannJsonConverter> {
    public:
        virtual ~NlohmannJsonObj<T>() = default;
    private:
        NlohmannJsonObj() = default;
        friend T;
    };
}

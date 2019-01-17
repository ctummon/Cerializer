#pragma once

#include "Cerializer/Serializable.h"
#include "Cerializer/RapidJsonConverter.h"

namespace Cerial {

    template <typename T>
    class RapidJsonObj : public Serializable::Object<T, RapidJsonConverter> {
    public:
        virtual ~RapidJsonObj<T>() = default;
    };
}
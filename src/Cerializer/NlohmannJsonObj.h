#pragma once

#include "NlohmannJsonConverter.h"
#include "Serializable.h"

namespace Cerializer {
template<typename T>
class NlohmannJsonObj : public Serializable::Object<T, NlohmannJsonConverter>
{
  public:
    virtual ~NlohmannJsonObj<T>() = default;

  private:
    NlohmannJsonObj() = default;
    friend T;
};
} // namespace Cerializer

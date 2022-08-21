#pragma once

#include "Cerializer/CppRestJsonConverter.h"
#include "Cerializer/Serializable.h"

namespace Cerializer {
template<typename BaseClass>
class CppRestJsonObj
  : public Serializable::Object<BaseClass, CppRestJsonConverter>
{
  public:
    virtual ~CppRestJsonObj<T>() = default;

  private:
    CppRestJsonObj() = default;
    friend BaseClass;
};
} // namespace Cerializer
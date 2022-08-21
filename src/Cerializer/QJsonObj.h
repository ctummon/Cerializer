#pragma once

#include "Cerializer/QJsonConverter.h"
#include "Cerializer/Serializable.h"

namespace Cerializer {
template<typename DerivedClass>
class QJsonObj : public Serializable::Object<DerivedClass, QJsonConverter>
{
  public:
    virtual ~QJsonObj<DerivedClass>() = default;

  private:
    QJsonObj() = default;
    friend DerivedClass;
};
} // namespace Cerializer
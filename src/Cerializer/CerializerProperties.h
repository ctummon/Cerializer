#pragma once

namespace Cerializer {
template<class DerivedClass>
class Properties
{
  public:
    virtual ~Properties<DerivedClass>() = default;

  protected:
    template<typename T, typename IdentiferType>
    struct PropertyImpl
    {
        constexpr PropertyImpl(T DerivedClass::*aMember,
          IdentiferType const* aName)
          : member{ aMember }
          , name{ aName }
        {}

        using Type = T;

        T DerivedClass::*member;
        IdentiferType const* name;
    };

    template<typename DClass = DerivedClass, typename T, typename IdentiferType>
    static constexpr auto property(T DClass::*member, IdentiferType const* name)
    {
        return PropertyImpl<T, IdentiferType>{ member, name };
    }

    using DerivedClassAlias = DerivedClass;

  protected:
    Properties() = default;
    friend DerivedClass;
};
} // namespace Cerializer
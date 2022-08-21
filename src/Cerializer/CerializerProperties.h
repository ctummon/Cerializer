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
          IdentiferType const* aName,
          bool aField_check = false)
          : member{ aMember }
          , name{ aName }
          , field_check{ aField_check }
        {}

        using Type = T;

        T DerivedClass::*member;
        IdentiferType const* name;
        bool field_check;
    };

    template<typename DClass = DerivedClass, typename T, typename IdentiferType>
    static constexpr auto property(T DClass::*member,
      IdentiferType const* name,
      bool field_check = false)
    {
        return PropertyImpl<T, IdentiferType>{ member, name, field_check };
    }

    using DerivedClassAlias = typename DerivedClass;

  protected:
    Properties() = default;
    friend DerivedClass;
};
} // namespace Cerializer
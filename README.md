# What is Cerializer?

Cerializer is a set of headers that help take the pain out of JSON serialization using compile time reflection.

You no longer need to manually parse JSON anymore.
All you need is your favourite JSON serializer.

So far only Qt and Cpprestsdk implementions are supported. I plan on adding as many as I can, feel free to contribute!

## Getting started:

Copy the Cerializer folder into your project and include the JsonObj file of your choice.

## Example Usage:

``` C++
#include "Cerializer\CppRestJsonObj.h"

class Hand : public Cerial::CppRestJsonObj<Hand>
{
public:
    int fingers{ 0 };
    
   S_PROPERTIES_BEGIN
        CPPREST_S_PROPERTY(Hand, fingers),
   S_PROPERTIES_END
};

class Person : public Cerial::CppRestJsonObj<Person>
{
public:
    std::string name;
    int age{ 0 };
    std::vector<Hand> hands{ 0 };

    
   S_PROPERTIES_BEGIN
        //Note for other JSON serializers use S_PROPERTY instead, see tests for examples.
        CPPREST_S_PROPERTY(Person, name),
        CPPREST_S_PROPERTY(Person, age),
        CPPREST_S_PROPERTY(Person, hands),
   S_PROPERTIES_END
};

...

Person bob;

bob.name = "bob";
bob.age = 35;
bob.hands = { {5}, {5} };

web::json::value bobJson = bob.toJson();
utility::string_t bobJsonStr = bobJson.serialize();
web::json::value bobJsonClone = web::json::value::parse(bobJsonStr);
Person bobsClone = Person::fromJson(bobJsonClone);

std::cout << "Clone's name: " << // bob
bobsClone.name << " Age: " <<    // 35
bobsClone.age << " Hands: " <<   // 2
bobsClone.hands.size() << std::endl;
```

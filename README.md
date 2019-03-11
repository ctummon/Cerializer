# What is Cerializer?

Cerializer is a set of headers that help take the pain out of JSON serialization in C++ with the help of compile time 'reflection'.

You no longer need to waste time manually parsing JSON.
All you need is your favourite JSON library.

So far  Qt , Cpprestsdk , and RapidJson implementations are supported. I plan on adding as many as I can, feel free to contribute!

## Getting started:

Copy the Cerializer folder into your project and include the JsonObj file of your choice.

(Currently only tested on Visual Studio 2017. I need to get some CI up and running...)

## Example Usage (cpprest):

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

```

## Example Usage (rapidjson):
``` C++
#include "Cerializer\RapidJsonObj.h"

class Hand : public Cerial::RapidJsonObj<Hand>
{
public:
    int fingers{ 0 };
    
   S_PROPERTIES_BEGIN
       S_PROPERTY(Hand, fingers),
   S_PROPERTIES_END
};

class Person : public Cerial::RapidJsonObj<Person>
{
public:
    std::string name;
    int age{ 0 };
    std::vector<Hand> hands{ 0 };

    
   S_PROPERTIES_BEGIN
        S_PROPERTY(Person, name),
        S_PROPERTY(Person, age),
        S_PROPERTY(Person, hands),
   S_PROPERTIES_END
};

...

Person bob;

bob.name = "bob";
bob.age = 35;
bob.hands = { {5}, {5} };


std::string bobJsonStr = bob.toJsonStr();
rapidjson::Document bobJsonDocClone;
bobJsonDocClone.Parse(bobJsonStr.c_str());
Person bobsClone = Person::fromJson(bobJsonDocClone);

std::cout << "Clone's name: " << // bob
bobsClone.name << " Age: " <<    // 35
bobsClone.age << " Hands: " <<   // 2
bobsClone.hands.size() << std::endl;
```

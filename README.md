[![Build Status](https://travis-ci.com/CathalT/Cerializer.svg?branch=master)](https://travis-ci.com/CathalT/Cerializer)

# What is Cerializer (C++17) ?

Cerializer is a set of headers that help take the pain out of JSON serialization in C++ with the help of compile time 'reflection'.

You no longer need to waste time manually parsing JSON.
All you need is your favourite JSON library.

So far the following implmentations are supported:

- Qt
- Cpprestsdk
- RapidJson
- nlohmann::json
- simdjson

After the performance results I'm probably going to hold off on adding any other libaries for now.
In the future I'll have a look at some of the more performant ones and see if I can find a more efficient solution.

## Getting started:

Copy the Cerializer folder into your project and include the JsonObj file of your choice.

Builds on VS2017, Clang and GCC. C++ 17 is required.

## Example Usage (cpprest):

``` C++
#include "Cerializer\CppRestJsonObj.h"

class Hand : public Cerializer::CppRestJsonObj<Hand>
{
public:
    int fingers{ 0 };
    
   S_PROPERTIES_BEGIN
        CPPREST_S_PROPERTY(fingers),
   S_PROPERTIES_END
};

class Person : public Cerializer::CppRestJsonObj<Person>
{
public:
    std::string name;
    int age{ 0 };
    std::vector<Hand> hands{ 0 };
    std::optional<int> legs; // Will return false if object is never found in Json
    std::string mFakeNameAlias;
    
   S_PROPERTIES_BEGIN
        //Note for other JSON serializers use S_PROPERTY instead, see tests for examples.
        CPPREST_S_PROPERTY(name),
        CPPREST_S_PROPERTY(age),
        CPPREST_S_PROPERTY(hands),
        CPPREST_S_PROPERTY(legs),
        CPPREST_S_PROPERTY_ALIAS(mFakeNameAlias, "fakeName") // Json object name
   S_PROPERTIES_END
};

```

## Example Usage (rapidjson):
``` C++
#include "Cerializer\RapidJsonObj.h"

class Hand : public Cerializer::RapidJsonObj<Hand>
{
public:
    int fingers{ 0 };
    
   S_PROPERTIES_BEGIN
       S_PROPERTY(fingers),
   S_PROPERTIES_END
};

class Person : public Cerializer::RapidJsonObj<Person>
{
public:
    std::string name;
    int age{ 0 };
    std::vector<Hand> hands{ 0 };
    std::optional<int> legs; // Will return false if object is never found in Json
    std::string mFakeNameAlias;

   S_PROPERTIES_BEGIN
        S_PROPERTY(name),
        S_PROPERTY(age),
        S_PROPERTY(hands),
        S_PROPERTY(legs),
        S_PROPERTY_ALIAS(mFakeNameAlias, "fakeName") // Json object name
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

## Basic Performance Tests:
Adding Cerializer to the original nativejson-benchmark tests proved to be more work than I was willing to put in.
So instead I forked the codebase and created a stripped down [version](https://github.com/CathalT/nativejson-benchmark/tree/cerializer_tests), it still gives a general idea of how much overhead is involved.
Only two tests were benchmarked , parse and stringify, everything else should be the exact same, i.e. handled by the JSON library.

### Parse ( String -> JSON DOM object -> C++ Struct )

Note: I used two JSON files of pre-generated (using Cerializer) data 1,582KB and 17,877KB.

   Library    | Total Time | Base Parse Time | Cerializer Time | Additional Overhead
--------------|------------|-----------------|-----------------|--------------------
  cpprestsdk  |    619ms   |      235ms      |      384ms      |        x1.63
  nlohmann    |    492ms   |      167ms      |      325ms      |        x1.95
  Qt          |    230ms   |      103ms      |      127ms      |        x1.23
  RapidJson   |     66ms   |       34ms      |       32ms      |        x0.94
  
  ![Parse time](https://github.com/CathalT/nativejson-benchmark/blob/cerializer_tests/sample/performance_unknown_win32_vc2013_1.%20Parse_Time%20(ms).jpeg)
  
Not as fast as I would have hoped, but I can't tell if its the underlying library calls that are slow or if I'm using the APIs inefficiently, but I don't think its the glue code itself.
The RapidJson implementation gives me the results I would expect, almost a 1:1 ratio, I'm essentially building the JSON object in memory twice, once for the DOM and once for the struct.

### Stringify (C++ Struct -> Build JSON DOM -> String) 
Note: RapidJson writes directly to StringBuffer using the Writer class.
  
   Library    | Total Time | Base Stringify Time | Cerializer Time | Additional Overhead
--------------|------------|---------------------|-----------------|--------------------
  cpprestsdk  |    587ms   |         78ms        |      509ms      |        x6.53
  nlohmann    |    565ms   |        195ms        |      370ms      |        x1.90
  Qt          |    421ms   |        271ms        |      150ms      |        x0.55
  RapidJson   |     80ms   |         26ms        |       57ms      |        x2.19
  
![Stringify time](https://github.com/CathalT/nativejson-benchmark/blob/cerializer_tests/sample/performance_unknown_win32_vc2013_2.%20Stringify_Time%20(ms).jpeg)

Some interesting results, the main issue is creating the in-memory representation seems quite slow. (Again, could be my usage of the APIs that is the issue)
I don't believe the original nativejson benchmarks actually measure generating the DOM from C++.
Quite surprised at the RapidJson result as I am using their Writer and String buffer, assuming there are some improvements to be made on my side.

**Conclusion:**
In future Im probably going to use a combination of Cerializer and RapidJson as it still performs well enough for my needs.
The ideal scenario would to have Cerializer parse directly from a string -> C++ struct, but thats a different story altogether.

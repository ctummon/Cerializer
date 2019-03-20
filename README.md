# What is Cerializer (C++14) ?

Cerializer is a set of headers that help take the pain out of JSON serialization in C++ with the help of compile time 'reflection'.

You no longer need to waste time manually parsing JSON.
All you need is your favourite JSON library.

So far the following implmentations are supported:

- Qt
- Cpprestsdk
- RapidJson
- nlohmann::json

After the performance results I'm probably going to hold off on adding any other libaries for now.
In the future I'll have a look at some of the more performant ones and see if I can find a more efficient solution.

## Getting started:

Copy the Cerializer folder into your project and include the JsonObj file of your choice.

Currently only tested on VS2017 (Which means C++14, theoretically C++11 should work, need to test on clang/gcc).
It will not compile on VS2015.
Next steps are to get some CI up and running for clang/gcc on Mac/Linux.

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

## Basic Performance Tests:
Adding Cerializer to the original nativejson-benchmark tests proved to be more work than I was willing to put in.
So instead I forked the codebase and created a stripped down [version](https://github.com/CathalT/nativejson-benchmark/tree/cerializer_tests), it still gives a general idea of how much overhead is involved.
Only two tests were benchmarked , parse and stringify, everything else should be the exact same, i.e. handled by the JSON library.

### Parse ( String -> JSON DOM object -> C++ Struct )

Note: I used two JSON files of pre-generated (using Cerializer) data 1,582KB and 17,877KB.

   Library    | Total Time | Base Parse Time | Cerializer Time | Additional Overhead
--------------|------------|-----------------|-----------------|--------------------
  cpprestsdk  |    661ms   |      234ms      |      427ms      |        x1.82
  nlohmann    |    517ms   |      163ms      |      354ms      |        x2.17
  Qt          |    274ms   |      103ms      |      171ms      |        x1.68
  RapidJson   |     78ms   |       37ms      |       41ms      |        x1.11
  
  ![Parse time](https://github.com/CathalT/nativejson-benchmark/blob/cerializer_tests/sample/performance_unknown_win32_vc2013_1.%20Parse_Time%20(ms).jpeg)
  
Not as fast as I would have hoped, but I can't tell if its the underlying library calls that are slow or if I'm using the APIs inefficiently, but I don't think its the glue code itself.
The RapidJson implementation gives me the results I would expect, almost a 1:1 ratio, I'm essentially building the JSON object in memory twice, once for the DOM and once for the struct.

### Stringify (C++ Struct -> Build JSON DOM -> String) 
Note: RapidJson writes directly to StringBuffer using the Writer class.
  
   Library    | Total Time | Base Stringify Time | Cerializer Time | Additional Overhead
--------------|------------|---------------------|-----------------|--------------------
  cpprestsdk  |    601ms   |         77ms        |      524ms      |        x6.81
  nlohmann    |    560ms   |        190ms        |      370ms      |        x1.95
  Qt          |    429ms   |        275ms        |      154ms      |        x0.56
  RapidJson   |     79ms   |         26ms        |       53ms      |        x2.04
  
![Stringify time](https://github.com/CathalT/nativejson-benchmark/blob/cerializer_tests/sample/performance_unknown_win32_vc2013_2.%20Stringify_Time%20(ms).jpeg)

Some interesting results, the main issue is creating the in-memory representation seems quite slow. (Again, could be my usage of the APIs that is the issue)
I don't believe the original nativejson benchmarks actually measure generating the DOM from C++.
Quite surprised at the RapidJson result as I am using their Writer and String buffer, assuming there are some improvements to be made on my side.

**Conclusion:**
In future Im probably going to use a combination of Cerializer and RapidJson as it still performs well enough for my needs.
The ideal scenario would to have Cerializer parse directly from a string -> C++ struct, but thats a different story altogether.

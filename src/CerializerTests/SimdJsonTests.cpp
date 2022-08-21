#include <simdjson.h>

#include <iostream>

#include "Cerializer/SimdJsonObj.h"
#include "catch.hpp"

namespace SIMDJsonTests {
class Hands : public Cerializer::SIMDJsonObj<Hands>
{
  public:
    std::set<int> fingers;
    float nails{ 0 };

    std::vector<double> knuckles;

    S_PROPERTIES_BEGIN
        S_PROPERTY(fingers)
        , S_PROPERTY(nails)
        , S_PROPERTY(knuckles)
    S_PROPERTIES_END
};

class Face : public Cerializer::SIMDJsonObj<Face>
{
  public:
    unsigned int eyes{ 0 };

    S_PROPERTIES_BEGIN
        S_PROPERTY(eyes)
    S_PROPERTIES_END
};

class Person : public Cerializer::SIMDJsonObj<Person>
{
  public:
    std::string name;
    std::wstring lastName;
    std::vector<Hands> hands;

    Face face;

    short foot{ 0 };
    unsigned short age{ 0 };
    int legs{ 0 };
    long long ageInMs{ 0 };
    unsigned int arms{ 0 };
    unsigned long hairs{ 0 };

    S_PROPERTIES_BEGIN
        S_PROPERTY(name)
        , S_PROPERTY(lastName)
        , S_PROPERTY(hands)
        , S_PROPERTY(face)
        , S_PROPERTY(foot)
        , S_PROPERTY(age)
        , S_PROPERTY(legs)
        , S_PROPERTY(ageInMs)
        , S_PROPERTY(arms)
        , S_PROPERTY(hairs)
    S_PROPERTIES_END
};

TEST_CASE("SIMDJson Serialization", "[SIMDJsonTests]")
{
    /*simdjson::dom::parser parser;
  simdjson::dom::element tweets = parser.load("twitter.json");
  std::cout << tweets["search_metadata"]["count"] << " results." << std::endl;

  simdjson::ondemand::parser ondemand_parser;
  auto json = R"({"test":"hello"})"_padded;
  simdjson::ondemand::document doc = ondemand_parser.iterate(json);
  std::cout << "test: " << doc["test"] << std::endl;*/

    simdjson::ondemand::parser parser;

    Person bob;

    bob.age = 14;
    bob.ageInMs = 124011515;
    bob.arms = 3;
    bob.foot = 1;
    bob.face.eyes = 2;
    bob.hairs = 1414;
    bob.hands = { Hands(), Hands(), Hands() };
    bob.hands[0].fingers.insert(9);
    bob.hands[1].knuckles = { 1.5, 2, 3.7 };
    bob.hands[2].nails = 7;
    bob.lastName = L"samson";
    bob.name = "bobby";

    auto bobJsonStr = bob.toJsonStr();
    Person bobsClone = Person::fromJson(parser, bobJsonStr);

    REQUIRE(bob.age == bobsClone.age);
    REQUIRE(bob.ageInMs == bobsClone.ageInMs);
    REQUIRE(bob.arms == bobsClone.arms);
    REQUIRE(bob.face.eyes == bobsClone.face.eyes);
    REQUIRE(bob.foot == bobsClone.foot);
    REQUIRE(bob.hairs == bobsClone.hairs);
    REQUIRE(bob.hands.size() == bobsClone.hands.size());
    REQUIRE(bob.hands[0].fingers.size() == bobsClone.hands[0].fingers.size());
    REQUIRE(bob.hands[1].knuckles == bobsClone.hands[1].knuckles);
    REQUIRE(bob.hands[2].nails == bobsClone.hands[2].nails);
    REQUIRE(bob.lastName == bobsClone.lastName);
    REQUIRE(bob.name == bobsClone.name);
}
} // namespace SIMDJsonTests

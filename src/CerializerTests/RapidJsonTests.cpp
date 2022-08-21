#include <rapidjson/document.h>

#include "Cerializer/RapidJsonObj.h"
#include "Utils/TestUtils.h"
#include "catch.hpp"

namespace RapidJsonTests {
class Hands : public Cerializer::RapidJsonObj<Hands>
{
  public:
    std::set<int> fingers;
    float nails{ 0 };
    bool hasFingers{ false };

    std::vector<double> knuckles;

    S_PROPERTIES_BEGIN
        S_PROPERTY(fingers)
        , S_FIELD_CHECK(fingers)
        , S_PROPERTY(nails)
        , S_PROPERTY(knuckles)
    S_PROPERTIES_END
};

class Face : public Cerializer::RapidJsonObj<Face>
{
  public:
    unsigned int eyes{ 0 };

    S_PROPERTIES_BEGIN
        S_PROPERTY(eyes)
    S_PROPERTIES_END
};

class Person : public Cerializer::RapidJsonObj<Person>
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

TEST_CASE("RapidJson Serialization", "[RapidJsonTests]")
{
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

    std::string bobJsonStr = bob.toJsonStr();

    rapidjson::Document bobJsonDocClone;
    bobJsonDocClone.Parse(bobJsonStr.c_str());

    Person bobsClone = Person::fromJson(bobJsonDocClone);

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

class TestFieldCheck : public Cerializer::RapidJsonObj<TestFieldCheck>
{
  public:
    bool Name{};
    bool Age{};
    bool HeightInMeters{};
    bool Surname{};

    S_PROPERTIES_BEGIN
        S_FIELD_CHECK(Name)
        , S_FIELD_CHECK(Age)
        , S_FIELD_CHECK(HeightInMeters)
        , S_FIELD_CHECK(Surname)
    S_PROPERTIES_END
};

TEST_CASE("RapidJson field exists check", "[RapidJsonTests]")
{
    rapidjson::Document jsonDoc;
    jsonDoc.Parse(Cerializer::getTestJson().c_str());
    const auto testFields = TestFieldCheck::fromJson(jsonDoc);

    REQUIRE(testFields.Name);
    REQUIRE(testFields.Age);
    REQUIRE(testFields.HeightInMeters);
    REQUIRE_FALSE(testFields.Surname);
}
} // namespace RapidJsonTests

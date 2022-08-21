
#include "Cerializer/CppRestJsonObj.h"
#include "Utils/TestUtils.h"

#include "catch.hpp"

#include <cpprest/json.h>
namespace CppRestObjTests {
class Hands : public Cerializer::CppRestJsonObj<Hands>
{
  public:
    std::set<int> fingers;
    float nails{ 0 };

    std::vector<double> knuckles;

    S_PROPERTIES_BEGIN
        CPPREST_S_PROPERTY(fingers)
        , CPPREST_S_PROPERTY(nails)
        , CPPREST_S_PROPERTY(knuckles)
    S_PROPERTIES_END
};

class Face : public Cerializer::CppRestJsonObj<Face>
{
  public:
    unsigned int eyes{ 0 };

    S_PROPERTIES_BEGIN
        CPPREST_S_PROPERTY(eyes)
    S_PROPERTIES_END
};

class Person : public Cerializer::CppRestJsonObj<Person>
{
  public:
    std::string name;
    std::wstring lastName;
    std::vector<Hands> hands;
    std::string mFakeNameAlias;
    Face face;

    short foot{ 0 };
    unsigned short age{ 0 };
    int legs{ 0 };
    long long ageInMs{ 0 };
    unsigned int arms{ 0 };
    unsigned long hairs{ 0 };

    S_PROPERTIES_BEGIN
        CPPREST_S_PROPERTY(name)
        , CPPREST_S_PROPERTY(lastName)
        , CPPREST_S_PROPERTY(hands)
        , CPPREST_S_PROPERTY(face)
        , CPPREST_S_PROPERTY(foot)
        , CPPREST_S_PROPERTY(age)
        , CPPREST_S_PROPERTY(legs)
        , CPPREST_S_PROPERTY(ageInMs)
        , CPPREST_S_PROPERTY(arms)
        , CPPREST_S_PROPERTY(hairs)
        , CPPREST_S_PROPERTY_ALIAS(mFakeNameAlias, U("fakeName"))
    S_PROPERTIES_END
};

TEST_CASE("CppRestSdk Alias Property Test", "[CppRestTests]")
{
    Person bob;
    auto p = std::get<10>(bob.getProperties());
    // need utility str compare
    // REQUIRE(strcmp(p.name, "fakeName") == 0);
    // REQUIRE(&p.mFakeNameAlias == p.member);
}

TEST_CASE("CppRestSdk Json Serialization", "[CppRestTests]")
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

    web::json::value bobJson = bob.toJson();
    utility::string_t bobJsonStr = bobJson.serialize();
    web::json::value bobJsonClone = web::json::value::parse(bobJsonStr);
    Person bobsClone = Person::fromJson(bobJsonClone);

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

class FieldsExistTestCase
  : public Cerializer::CppRestJsonObj<FieldsExistTestCase>
{
  public:
    std::optional<std::string> Name;
    std::optional<int> Age{};
    std::optional<double> HeightInMeters{};
    std::optional<std::string> Surname;

    S_PROPERTIES_BEGIN
        S_PROPERTY(Name)
        , S_PROPERTY(Age)
        , S_PROPERTY(HeightInMeters)
        , S_PROPERTY(Surname)
    S_PROPERTIES_END
};

TEST_CASE_METHOD(FieldsExistTestCase,
  "CppRest field exists check",
  "[CppRestTests]")
{
    web::json::value bobJsonClone =
      web::json::value::parse(Cerializer::getTestJson<utility::string_t>());
    const auto testFields = fromJson(jsonDoc);

    REQUIRE(testFields.Name);
    REQUIRE(testFields.Age);
    REQUIRE(testFields.HeightInMeters);
    REQUIRE_FALSE(testFields.Surname);
}

} // namespace CppRestObjTests

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
/*#include "Cerializer\RapidJsonObj.h"
#include "Cerializer\rapidjson\stringbuffer.h"
#include "Cerializer\rapidjson\writer.h"*/

#ifdef BUILD_QT_OBJ
#include "Cerializer\QJsonObj.h"
#include "QtCore\QJsonDocument.h"
#endif

#ifdef BUILD_CPPREST_OBJ
#include "Cerializer\CppRestJsonObj.h"
#include <cpprest\json.h>
#endif

#include <set>
/*
namespace rapidJsonTests {
    class Hands : public Cerial::RapidJsonObj<Hands>
    {
    public:
        std::set<int> fingers;
        float nails{ 0 };

        std::vector<int> knuckles;

        S_PROPERTIES_BEGIN
            S_PROPERTY(Hands, fingers),
            S_PROPERTY(Hands, nails),
            S_PROPERTY(Hands, knuckles)
        S_PROPERTIES_END
    };

    class Face : public Cerial::RapidJsonObj<Face>
    {
    public:
        int eyes{ 0 };

        S_PROPERTIES_BEGIN
            S_PROPERTY(Face, eyes)
        S_PROPERTIES_END
    };

    class Person : public Cerial::RapidJsonObj<Person>
    {
    public:
        std::string name;
        std::wstring lastName;
        std::vector<Hands> hands;

        Face face;

        unsigned short age{ 0 };
        int legs{ 0 };
        long long ageInMs{ 0 };
        unsigned int arms{ 0 };
        unsigned long hairs{ 0 };

        S_PROPERTIES_BEGIN
            S_PROPERTY(Person, name),
            S_PROPERTY(Person, lastName),
            S_PROPERTY(Person, hands),
            S_PROPERTY(Person, face),
            S_PROPERTY(Person, age),
            S_PROPERTY(Person, legs),
            S_PROPERTY(Person, ageInMs),
            S_PROPERTY(Person, arms),
            S_PROPERTY(Person, hairs)
       S_PROPERTIES_END
    };

    TEST_CASE("Rapid Json Serialization", "[JsonTests]")
    {
        Person bob;

        bob.age = 14;
        bob.ageInMs = 124011515;
        bob.arms = 3;
        bob.face.eyes = 2;
        bob.hairs = 1414;
        bob.hands = { Hands(), Hands(), Hands() };
        bob.hands[0].fingers.insert(9);
        bob.hands[1].knuckles = { 1,2,3 };
        bob.hands[2].nails = 7;
        bob.lastName = L"timOThy";
        bob.name = "bobsans";

        rapidjson::Document bobDoc = bob.toJson();
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        bobDoc.Accept(writer);

        const char* stringBob = buffer.GetString();

        rapidjson::Document bobsDocClone;
        bobsDocClone.Parse(stringBob);

        Person bobsClone = Person::fromJson(bobsDocClone);

        REQUIRE(bob.age == bobsClone.age);
        REQUIRE(bob.ageInMs == bobsClone.ageInMs);
        REQUIRE(bob.arms == bobsClone.arms);
        REQUIRE(bob.face.eyes == bobsClone.face.eyes);
        REQUIRE(bob.hairs == bobsClone.hairs);
        REQUIRE(bob.hands.size() == bobsClone.hands.size());
        REQUIRE(bob.hands[0].fingers.size() == bobsClone.hands[0].fingers.size());
        REQUIRE(bob.hands[1].knuckles == bobsClone.hands[1].knuckles);
        REQUIRE(bob.hands[2].nails == bobsClone.hands[2].nails);
        REQUIRE(bob.lastName == bobsClone.lastName);
        REQUIRE(bob.name == bobsClone.name);
    }
}
*/
#ifdef BUILD_QT_OBJ
namespace QtJsonTests {
    class Hands : public Cerial::QJsonObj<Hands>
    {
    public:
        std::set<int> fingers;
        float nails{ 0 };

        std::vector<int> knuckles;

        S_PROPERTIES_BEGIN
            S_PROPERTY(Hands, fingers),
            S_PROPERTY(Hands, nails),
            S_PROPERTY(Hands, knuckles)
        S_PROPERTIES_END
    };

    class Face : public Cerial::QJsonObj<Face>
    {
    public:
        int eyes{ 0 };

        S_PROPERTIES_BEGIN
            S_PROPERTY(Face, eyes)
        S_PROPERTIES_END
    };

    class Person : public Cerial::QJsonObj<Person>
    {
    public:
        std::string name;
        std::wstring lastName;
        std::vector<Hands> hands;

        Face face;

        unsigned short age{ 0 };
        int legs{ 0 };
        long long ageInMs{ 0 };
        unsigned int arms{ 0 };
        unsigned long hairs{ 0 };

        S_PROPERTIES_BEGIN
            S_PROPERTY(Person, name),
            S_PROPERTY(Person, lastName),
            S_PROPERTY(Person, hands),
            S_PROPERTY(Person, face),
            S_PROPERTY(Person, age),
            S_PROPERTY(Person, legs),
            S_PROPERTY(Person, ageInMs),
            S_PROPERTY(Person, arms),
            S_PROPERTY(Person, hairs)
        S_PROPERTIES_END
    };

    TEST_CASE("Qt Json Serialization", "[QJsonTests]")
    {
        Person bob;

        bob.age = 14;
        bob.ageInMs = 124011515;
        bob.arms = 3;
        bob.face.eyes = 2;
        bob.hairs = 1414;
        bob.hands = { Hands(), Hands(), Hands() };
        bob.hands[0].fingers.insert(9);
        bob.hands[1].knuckles = { 1,2,3 };
        bob.hands[2].nails = 7;
        bob.lastName = L"timOThy";
        bob.name = "bobsans";

        QJsonObject bobJson = bob.toJson();
        QJsonDocument bobJsonDoc(bobJson);
        QString bobString(bobJsonDoc.toJson(QJsonDocument::Compact));

        QJsonDocument bobJsonDocClone = QJsonDocument::fromJson(bobString.toUtf8());
        
        Person bobsClone = Person::fromJson(bobJsonDocClone.object());

        REQUIRE(bob.age == bobsClone.age);
        REQUIRE(bob.ageInMs == bobsClone.ageInMs);
        REQUIRE(bob.arms == bobsClone.arms);
        REQUIRE(bob.face.eyes == bobsClone.face.eyes);
        REQUIRE(bob.hairs == bobsClone.hairs);
        REQUIRE(bob.hands.size() == bobsClone.hands.size());
        REQUIRE(bob.hands[0].fingers.size() == bobsClone.hands[0].fingers.size());
        REQUIRE(bob.hands[1].knuckles == bobsClone.hands[1].knuckles);
        REQUIRE(bob.hands[2].nails == bobsClone.hands[2].nails);
        REQUIRE(bob.lastName == bobsClone.lastName);
        REQUIRE(bob.name == bobsClone.name);
    }
}
#endif

#ifdef BUILD_CPPREST_OBJ
namespace CppRestObjTests {
    class Hands : public Cerial::CppRestJsonObj<Hands>
    {
    public:
        std::set<int> fingers;
        float nails{ 0 };

        std::vector<int> knuckles;

        S_PROPERTIES_BEGIN
            CPPREST_S_PROPERTY(Hands, fingers),
            CPPREST_S_PROPERTY(Hands, nails),
            CPPREST_S_PROPERTY(Hands, knuckles)
        S_PROPERTIES_END
    };

    class Face : public Cerial::CppRestJsonObj<Face>
    {
    public:
        int eyes{ 0 };

        S_PROPERTIES_BEGIN
            CPPREST_S_PROPERTY(Face, eyes)
        S_PROPERTIES_END
    };

    class Person : public Cerial::CppRestJsonObj<Person>
    {
    public:
        std::string name;
        std::wstring lastName;
        std::vector<Hands> hands;

        Face face;

        unsigned short age{ 0 };
        int legs{ 0 };
        long long ageInMs{ 0 };
        unsigned int arms{ 0 };
        unsigned long hairs{ 0 };

        S_PROPERTIES_BEGIN
            CPPREST_S_PROPERTY(Person, name),
            CPPREST_S_PROPERTY(Person, lastName),
            CPPREST_S_PROPERTY(Person, hands),
            CPPREST_S_PROPERTY(Person, face),
            CPPREST_S_PROPERTY(Person, age),
            CPPREST_S_PROPERTY(Person, legs),
            CPPREST_S_PROPERTY(Person, ageInMs),
            CPPREST_S_PROPERTY(Person, arms),
            CPPREST_S_PROPERTY(Person, hairs)
       S_PROPERTIES_END
    };

    TEST_CASE("CppRestSdk Json Serialization", "[CppRestTests]")
    {
        Person bob;

        bob.age = 14;
        bob.ageInMs = 124011515;
        bob.arms = 3;
        bob.face.eyes = 2;
        bob.hairs = 1414;
        bob.hands = { Hands(), Hands(), Hands() };
        bob.hands[0].fingers.insert(9);
        bob.hands[1].knuckles = { 1,2,3 };
        bob.hands[2].nails = 7;
        bob.lastName = L"timOThy";
        bob.name = "bobsans";

        web::json::value bobJson = bob.toJson();
        utility::string_t bobJsonStr = bobJson.serialize();
        web::json::value bobJsonClone = web::json::value::parse(bobJsonStr);
        Person bobsClone = Person::fromJson(bobJsonClone);

        REQUIRE(bob.age == bobsClone.age);
        REQUIRE(bob.ageInMs == bobsClone.ageInMs);
        REQUIRE(bob.arms == bobsClone.arms);
        REQUIRE(bob.face.eyes == bobsClone.face.eyes);
        REQUIRE(bob.hairs == bobsClone.hairs);
        REQUIRE(bob.hands.size() == bobsClone.hands.size());
        REQUIRE(bob.hands[0].fingers.size() == bobsClone.hands[0].fingers.size());
        REQUIRE(bob.hands[1].knuckles == bobsClone.hands[1].knuckles);
        REQUIRE(bob.hands[2].nails == bobsClone.hands[2].nails);
        REQUIRE(bob.lastName == bobsClone.lastName);
        REQUIRE(bob.name == bobsClone.name);
    }
}
#endif
#include "catch.hpp"

#include "Cerializer/QJsonObj.h"
#include "QtCore/QJsonDocument.h"

namespace QtJsonTests {
    class Hands : public Cerial::QJsonObj<Hands>
    {
    public:
        std::set<int> fingers;
        float nails{ 0 };

        std::vector<double> knuckles;

        S_PROPERTIES_BEGIN
            S_PROPERTY(Hands, fingers),
            S_PROPERTY(Hands, nails),
            S_PROPERTY(Hands, knuckles)
        S_PROPERTIES_END
    };

    class Face : public Cerial::QJsonObj<Face>
    {
    public:
        unsigned int eyes{ 0 };

        S_PROPERTIES_BEGIN
            S_PROPERTY(Face, eyes)
        S_PROPERTIES_END
    };

    class Test : public Cerial::QJsonObj<Face>
    {};
    class Person : public Cerial::QJsonObj<Person>
    {
    public:
        std::string name;
        std::wstring lastName;
        std::vector<Hands> hands;
        QString testQStr;

        Face face;

        short foot{ 0 };
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
            S_PROPERTY(Person, foot),
            S_PROPERTY(Person, age),
            S_PROPERTY(Person, legs),
            S_PROPERTY(Person, ageInMs),
            S_PROPERTY(Person, arms),
            S_PROPERTY(Person, testQStr),
            S_PROPERTY(Person, hairs)
        S_PROPERTIES_END
    };

    TEST_CASE("Qt Json Serialization", "[QJsonTests]")
    {
        Person bob;

        bob.testQStr = QStringLiteral("TestQStr");
        bob.age = 14;
        bob.ageInMs = 124011515;
        bob.arms = 3;
        bob.face.eyes = 2;
        bob.foot = 1;
        bob.hairs = 1414;
        bob.hands = { Hands(), Hands(), Hands() };
        bob.hands[0].fingers.insert(9);
        bob.hands[1].knuckles = { 1.5,2,3.7 };
        bob.hands[2].nails = 7;
        bob.lastName = L"samson";
        bob.name = "bobby";

        QJsonObject bobJson = bob.toJson();
        QJsonDocument bobJsonDoc(bobJson);
        QString bobString(bobJsonDoc.toJson(QJsonDocument::Compact));

        QJsonDocument bobJsonDocClone = QJsonDocument::fromJson(bobString.toUtf8());
        
        Person bobsClone = Person::fromJson(bobJsonDocClone.object());

        REQUIRE(bob.testQStr == bobsClone.testQStr);
        REQUIRE(bob.age == bobsClone.age);
        REQUIRE(bob.ageInMs == bobsClone.ageInMs);
        REQUIRE(bob.arms == bobsClone.arms);
        REQUIRE(bob.foot == bobsClone.foot);
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

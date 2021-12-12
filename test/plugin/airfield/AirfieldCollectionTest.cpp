#include "airfield/AirfieldModel.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPluginTest::Airfield {

    class AirfieldCollectionTest : public testing::Test
    {
        public:
        AirfieldCollectionTest()
            : airfield1(std::make_shared<AirfieldModel>(1, "EGKK", std::make_unique<ControllerPositionHierarchy>())),
              airfield2(std::make_shared<AirfieldModel>(2, "EGLL", std::make_unique<ControllerPositionHierarchy>()))
        {
        }

        std::shared_ptr<AirfieldModel> airfield1;
        std::shared_ptr<AirfieldModel> airfield2;
        AirfieldCollection collection;
    };

    TEST_F(AirfieldCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.GetSize());
    }

    TEST_F(AirfieldCollectionTest, ItAddsAirfields)
    {

        collection.AddAirfield(airfield1);
        collection.AddAirfield(airfield2);

        EXPECT_EQ(2, collection.GetSize());
        EXPECT_EQ(airfield1, collection.FetchAirfieldByIcao("EGKK"));
        EXPECT_EQ(airfield2, collection.FetchAirfieldByIcao("EGLL"));
    }

    TEST_F(AirfieldCollectionTest, ItDoesntAddDuplicateAirfields)
    {

        collection.AddAirfield(airfield1);
        collection.AddAirfield(airfield1);
        collection.AddAirfield(airfield1);
        collection.AddAirfield(airfield2);
        collection.AddAirfield(airfield2);
        collection.AddAirfield(airfield2);

        EXPECT_EQ(2, collection.GetSize());
    }

    TEST_F(AirfieldCollectionTest, FetchAirfieldByIcaoReturnsNullPtrIfNotFound)
    {
        collection.AddAirfield(airfield1);
        collection.AddAirfield(airfield2);
        EXPECT_EQ(nullptr, collection.FetchAirfieldByIcao("EGBB"));
    }

    TEST_F(AirfieldCollectionTest, ItIteratesTheCollection)
    {
        collection.AddAirfield(airfield1);
        collection.AddAirfield(airfield2);

        std::vector<std::string> airfields;
        collection.ForEach([&airfields](const AirfieldModel& airfield) { airfields.push_back(airfield.Icao()); });

        std::vector<std::string> expected({"EGKK", "EGLL"});
        EXPECT_EQ(expected, airfields);
    }
} // namespace UKControllerPluginTest::Airfield

#include "airfield/AirfieldModel.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using ::testing::ElementsAre;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPluginTest::Airfield {

    class AirfieldModelTest : public testing::Test
    {
        public:
        AirfieldModelTest()
            : airfield(1, "EGKK", std::make_unique<ControllerPositionHierarchy>()),
              airfield2(2, "EGLL", std::make_unique<ControllerPositionHierarchy>()),
              airfield3(1, "EGBB", std::make_unique<ControllerPositionHierarchy>())
        {
        }

        AirfieldModel airfield;
        AirfieldModel airfield2;
        AirfieldModel airfield3;
    };

    TEST_F(AirfieldModelTest, ItHasAnId)
    {
        EXPECT_EQ(1, airfield.Id());
    }

    TEST_F(AirfieldModelTest, ItHasAnIcao)
    {
        EXPECT_EQ("EGKK", airfield.Icao());
    }

    TEST_F(AirfieldModelTest, GetOwnershipPresedenceReturnsOwnership)
    {
        EXPECT_EQ(0, airfield.TopDownOrder().CountPositions());
    }

    TEST_F(AirfieldModelTest, EqualityOperatorReturnsFalseDifferentId)
    {
        EXPECT_FALSE(airfield2 == airfield3);
    }

    TEST_F(AirfieldModelTest, EqualityOperatorReturnsTrueSameId)
    {
        EXPECT_TRUE(airfield == airfield3);
    }
} // namespace UKControllerPluginTest::Airfield

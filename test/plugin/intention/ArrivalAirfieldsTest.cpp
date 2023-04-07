#include "intention/ArrivalAirfields.h"

using UKControllerPlugin::IntentionCode::ArrivalAirfields;
using UKControllerPlugin::IntentionCode::Condition;

namespace UKControllerPluginTest::IntentionCode {
    class ArrivalAirfieldsTest : public testing::Test
    {
        public:
        ArrivalAirfieldsTest() : arrivalAirfields({"EGKK", "EGLL"})
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        ArrivalAirfields arrivalAirfields;
    };

    TEST_F(ArrivalAirfieldsTest, ItPassesIfAircraftArrivingAtSpecifiedAirfield)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("EGKK"));

        EXPECT_TRUE(arrivalAirfields.Passes(flightplan, radarTarget));
    }

    TEST_F(ArrivalAirfieldsTest, ItFailsIfAircraftNotArrivingAtSpecifiedAirfield)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("EGGD"));

        EXPECT_FALSE(arrivalAirfields.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode

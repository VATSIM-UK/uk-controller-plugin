#include "intention/ArrivalAirfieldPattern.h"

using UKControllerPlugin::IntentionCode::ArrivalAirfieldPattern;
using UKControllerPlugin::IntentionCode::Condition;

namespace UKControllerPluginTest::IntentionCode {
    class ArrivalAirfieldPatternTest : public testing::Test
    {
        public:
        ArrivalAirfieldPatternTest() : arrivalAirfields("EG")
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        ArrivalAirfieldPattern arrivalAirfields;
    };

    TEST_F(ArrivalAirfieldPatternTest, ItFailsIfNoArrivalAirfield)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return(""));

        EXPECT_FALSE(arrivalAirfields.Passes(flightplan, radarTarget));
    }

    TEST_F(ArrivalAirfieldPatternTest, ItFailsIfNoPatternMatch)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(2).WillRepeatedly(testing::Return("LFPG"));

        EXPECT_FALSE(arrivalAirfields.Passes(flightplan, radarTarget));
    }

    TEST_F(ArrivalAirfieldPatternTest, ItPassesOnPatternMatch)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(2).WillRepeatedly(testing::Return("EGGD"));

        EXPECT_TRUE(arrivalAirfields.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode

#include "intention/MaximumCruisingAltitude.h"

using UKControllerPlugin::IntentionCode::Condition;
using UKControllerPlugin::IntentionCode::MaximumCruisingAltitude;

namespace UKControllerPluginTest::IntentionCode {
    class MaximumCruisingAltitudeTest : public testing::Test
    {
        public:
        MaximumCruisingAltitudeTest() : maxAltitude(5000)
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        MaximumCruisingAltitude maxAltitude;
    };

    TEST_F(MaximumCruisingAltitudeTest, ItFailsIfCruisingAboveAltitude)
    {
        EXPECT_CALL(flightplan, GetCruiseLevel).Times(1).WillOnce(testing::Return(5001));

        EXPECT_FALSE(maxAltitude.Passes(flightplan, radarTarget));
    }

    TEST_F(MaximumCruisingAltitudeTest, ItPassesCruisingAtAltitude)
    {
        EXPECT_CALL(flightplan, GetCruiseLevel).Times(1).WillOnce(testing::Return(5000));

        EXPECT_TRUE(maxAltitude.Passes(flightplan, radarTarget));
    }

    TEST_F(MaximumCruisingAltitudeTest, ItPassesCruisingBelowAltitude)
    {
        EXPECT_CALL(flightplan, GetCruiseLevel).Times(1).WillOnce(testing::Return(4999));

        EXPECT_TRUE(maxAltitude.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode

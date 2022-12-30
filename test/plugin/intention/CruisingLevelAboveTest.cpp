#include "intention/CruisingLevelAbove.h"

using UKControllerPlugin::IntentionCode::Condition;
using UKControllerPlugin::IntentionCode::CruisingLevelAbove;

namespace UKControllerPluginTest::IntentionCode {
    class CruisingLevelAboveTest : public testing::Test
    {
        public:
        CruisingLevelAboveTest() : cruisingLevelAbove(5000)
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        CruisingLevelAbove cruisingLevelAbove;
    };

    TEST_F(CruisingLevelAboveTest, ItFailsIfCruisingBelowAltitude)
    {
        EXPECT_CALL(flightplan, GetCruiseLevel).Times(1).WillOnce(testing::Return(4999));

        EXPECT_FALSE(cruisingLevelAbove.Passes(flightplan, radarTarget));
    }

    TEST_F(CruisingLevelAboveTest, ItFailsIfCruisingAtAltitude)
    {
        EXPECT_CALL(flightplan, GetCruiseLevel).Times(1).WillOnce(testing::Return(5000));

        EXPECT_FALSE(cruisingLevelAbove.Passes(flightplan, radarTarget));
    }

    TEST_F(CruisingLevelAboveTest, ItPassesCruisingAboveAltitude)
    {
        EXPECT_CALL(flightplan, GetCruiseLevel).Times(1).WillOnce(testing::Return(5001));

        EXPECT_TRUE(cruisingLevelAbove.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode

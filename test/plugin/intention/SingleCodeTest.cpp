#include "intention/SingleCode.h"

using UKControllerPlugin::IntentionCode::SingleCode;

namespace UKControllerPluginTest::IntentionCode {
    class SingleCodeTest : public testing::Test
    {
        public:
        SingleCodeTest() : code("AM")
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        SingleCode code;
    };

    TEST_F(SingleCodeTest, ItReturnsCode)
    {
        EXPECT_EQ("AM", code.GenerateCode(flightplan));
    }
} // namespace UKControllerPluginTest::IntentionCode

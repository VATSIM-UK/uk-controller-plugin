#include "missedapproach/MissedApproachTriggeredMessage.h"
#include "time/ParseTimeStrings.h"

using UKControllerPlugin::MissedApproach::MissedApproachTriggeredMessage;
using UKControllerPlugin::Time::ParseTimeString;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachTriggeredMessageTest : public testing::Test
    {
        public:
        MissedApproachTriggeredMessageTest() : message("BAW123", true, ParseTimeString("2021-11-27 14:32:31"))
        {
        }

        MissedApproachTriggeredMessage message;
    };

    TEST_F(MissedApproachTriggeredMessageTest, ItConvertsToJson)
    {
        nlohmann::json expected = {
            {"type", "missed_approach_triggered"},
            {"version", 1},
            {"data", {{"callsign", "BAW123"}, {"triggered_by_user", true}, {"expires_at", "2021-11-27 14:32:31"}}}};

        EXPECT_EQ(expected, message.ToJson());
    }
} // namespace UKControllerPluginTest::MissedApproach

#include "stands/StandAssignedMessage.h"

using UKControllerPlugin::Stands::StandAssignedMessage;

namespace UKControllerPluginTests::Stands {
    class StandAssignedMessageTest : public testing::Test
    {
        public:
        StandAssignedMessageTest() : message("BAW123", "EGLL", "547")
        {
        }

        StandAssignedMessage message;
    };

    TEST_F(StandAssignedMessageTest, ItSerializesToJson)
    {
        nlohmann::json expected{
            {"type", "stand_assigned"},
            {"version", 1},
            {"data", {{"callsign", "BAW123"}, {"airfield", "EGLL"}, {"stand", "547"}}}};
        EXPECT_EQ(expected, message.ToJson());
    }
} // namespace UKControllerPluginTests::Stands

#include "stands/StandUnassignedMessage.h"

using UKControllerPlugin::Stands::StandUnassignedMessage;

namespace UKControllerPluginTests::Stands {
    class StandUnassignedMessageTest : public testing::Test
    {
        public:
        StandUnassignedMessageTest() : message("BAW123")
        {
        }

        StandUnassignedMessage message;
    };

    TEST_F(StandUnassignedMessageTest, ItSerializesToJson)
    {
        nlohmann::json expected{
            {"type", "stand_unassigned"},
            {"version", 1},
            {"data",
             {
                 {"callsign", "BAW123"},
             }}};
        EXPECT_EQ(expected, message.ToJson());
    }
} // namespace UKControllerPluginTests::Stands

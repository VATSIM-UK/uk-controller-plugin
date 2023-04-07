#include "intention/IntentionCodeUpdatedMessage.h"

using testing::Test;
using UKControllerPlugin::IntentionCode::IntentionCodeUpdatedMessage;

namespace UKControllerPluginTest::IntentionCode {

    class IntentionCodeUpdatedMessageTest : public Test
    {
        public:
        IntentionCodeUpdatedMessageTest() : message("BAW123", "REDFA", "C2"), message2("BAW123", "", "KK")
        {
        }

        IntentionCodeUpdatedMessage message;
        IntentionCodeUpdatedMessage message2;
    };

    TEST_F(IntentionCodeUpdatedMessageTest, ItConvertsToJson)
    {
        nlohmann::json expected = {
            {"type", "intention_code_updated"},
            {"version", 1},
            {"data", {{"callsign", "BAW123"}, {"exit_point", "REDFA"}, {"code", "C2"}}}};

        EXPECT_EQ(expected, message.ToJson());
    }

    TEST_F(IntentionCodeUpdatedMessageTest, ItConvertsToJsonWithNoExitPoint)
    {
        nlohmann::json expected = {
            {"type", "intention_code_updated"},
            {"version", 1},
            {"data", {{"callsign", "BAW123"}, {"exit_point", nlohmann::json::value_t::null}, {"code", "KK"}}}};

        EXPECT_EQ(expected, message2.ToJson());
    }
} // namespace UKControllerPluginTest::IntentionCode

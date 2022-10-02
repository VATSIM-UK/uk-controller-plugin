#include "handoff/HandoffFrequencyUpdatedMessage.h"

using testing::Test;
using UKControllerPlugin::Handoff::HandoffFrequencyUpdatedMessage;

namespace UKControllerPluginTest::Handoff {

    class HandoffFrequencyUpdatedEventTest : public Test
    {
        public:
        HandoffFrequencyUpdatedEventTest() : message("BAW123", "129.420")
        {
        }

        HandoffFrequencyUpdatedMessage message;
    };

    TEST_F(HandoffFrequencyUpdatedEventTest, ItConvertsToJson)
    {
        nlohmann::json expected = {
            {"type", "departure_frequency_updated"},
            {"version", 1},
            {"data", {{"callsign", "BAW123"}, {"frequency", "129.420"}}}};

        EXPECT_EQ(expected, message.ToJson());
    }
} // namespace UKControllerPluginTest::Handoff

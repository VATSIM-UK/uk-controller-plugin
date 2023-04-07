#include "integration/InitialisationFailureMessage.h"
#include "integration/MessageType.h"
#include "update/PluginVersion.h"

using testing::Test;
using UKControllerPlugin::Integration::InitialisationFailureMessage;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPlugin::Plugin::PluginVersion;

namespace UKControllerPluginTest::Integration {

    class InitialisationFailureMessageTest : public Test
    {
        public:
        InitialisationFailureMessageTest() : message("foo", std::vector<std::string>{"message1", "message2"})
        {
        }

        InitialisationFailureMessage message;
    };

    TEST_F(InitialisationFailureMessageTest, ItConvertsToJson)
    {
        nlohmann::json expected = {
            {"type", "initialisation_failure"}, {"id", "foo"}, {"version", 1}, {"errors", {"message1", "message2"}}};
        EXPECT_EQ(expected, message.ToJson());
    }
} // namespace UKControllerPluginTest::Integration

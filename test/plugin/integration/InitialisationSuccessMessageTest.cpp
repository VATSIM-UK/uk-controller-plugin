#include "integration/InitialisationSuccessMessage.h"
#include "integration/MessageType.h"
#include "update/PluginVersion.h"

using testing::Test;
using UKControllerPlugin::Integration::InitialisationSuccessMessage;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPlugin::Plugin::PluginVersion;

namespace UKControllerPluginTest::Integration {

    class InitialisationSuccessMessageTest : public Test
    {
        public:
        InitialisationSuccessMessageTest() : message("foo")
        {
        }

        InitialisationSuccessMessage message;
    };

    TEST_F(InitialisationSuccessMessageTest, ItConvertsToJson)
    {
        nlohmann::json expected = {
            {"type", "initialisation_success"},
            {"id", "foo"},
            {"version", 1},
            {"data", {{"ukcp_version", PluginVersion::version}}}};
        EXPECT_EQ(expected, message.ToJson());
    }
} // namespace UKControllerPluginTest::Integration

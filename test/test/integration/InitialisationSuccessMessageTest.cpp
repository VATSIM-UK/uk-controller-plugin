#include "pch/pch.h"
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
            InitialisationSuccessMessage message;
    };

    TEST_F(InitialisationSuccessMessageTest, ItConvertsToJson)
    {
        nlohmann::json expected = {
            {"type", "initialisation_success"},
            {"version", 1},
            {"data", {{"ukcp_version", PluginVersion::version}}}
        };
        EXPECT_EQ(expected, message.ToJson());
    }
} // namespace UKControllerPluginTest::Integration

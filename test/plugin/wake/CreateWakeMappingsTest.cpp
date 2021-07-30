#include "pch/pch.h"
#include "wake/CreateWakeMappings.h"
#include "wake/WakeCategoryMapper.h"
#include "message/UserMessager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "bootstrap/BootstrapWarningMessage.h"

using UKControllerPlugin::Wake::CreateWakeMappings;
using UKControllerPlugin::Wake::WakeCategoryMapper;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;

using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Wake {

        class CreateWakeMappingsTest : public Test
        {
            public:
                CreateWakeMappingsTest()
                    : messager(mockPlugin)
                {

                }

                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                UserMessager messager;
        };

        TEST_F(CreateWakeMappingsTest, ItCreatesAMapperFromData)
        {
            nlohmann::json data;
            data["A332"] = "H";
            data["B738"] = "LM";

            WakeCategoryMapper mapper = CreateWakeMappings(data, this->messager);
            EXPECT_EQ(2, mapper.Count());
        }

        TEST_F(CreateWakeMappingsTest, ItHandlesInvalidValues)
        {
            nlohmann::json data;
            data["A332"] = "H";
            data["B738"] = { "LM" };

            EXPECT_CALL(
                this->mockPlugin,
                ChatAreaMessage(
                    BootstrapWarningMessage::handler,
                    BootstrapWarningMessage::sender,
                    "Failed to load 1 wake categories",
                    true,
                    true,
                    true,
                    true,
                    true
                )
            )
                .Times(1);

            WakeCategoryMapper mapper = CreateWakeMappings(data, this->messager);
            EXPECT_EQ(1, mapper.Count());
        }

    }  // namespace Wake
}  // namespace UKControllerPluginTest

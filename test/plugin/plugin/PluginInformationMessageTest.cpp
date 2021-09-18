#include "plugin/PluginInformationMessage.h"
#include "mock/MockWinApi.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Plugin::PluginInformationMessage;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest {
    namespace Plugin {

        class PluginInformationMessageTest : public Test
        {
            public:
            PluginInformationMessageTest() : message(mockWindows, 123)
            {
            }

            NiceMock<MockWinApi> mockWindows;
            PluginInformationMessage message;
        };

        TEST_F(PluginInformationMessageTest, ItHasAConfigurationMenuItem)
        {
            PopupMenuItem expected;
            expected.firstValue = "About UKCP";
            expected.secondValue = "";
            expected.callbackFunctionId = 123;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.fixedPosition = false;
            EXPECT_EQ(expected, this->message.GetConfigurationMenuItem());
        }

        TEST_F(PluginInformationMessageTest, ProcessCommandReturnsFalseIfNotCommand)
        {
            EXPECT_FALSE(this->message.ProcessCommand(".ukcp notthis"));
        }

        TEST_F(PluginInformationMessageTest, ProcessCommandReturnsTrueIfCommand)
        {
            EXPECT_TRUE(this->message.ProcessCommand(".ukcp about"));
        }

        TEST_F(PluginInformationMessageTest, ProcessCommandShowsMessage)
        {
            EXPECT_CALL(this->mockWindows, OpenMessageBox(_, _, _)).Times(1);

            static_cast<void>(this->message.ProcessCommand(".ukcp about"));
        }

        TEST_F(PluginInformationMessageTest, ConfigureShowsMessage)
        {
            EXPECT_CALL(this->mockWindows, OpenMessageBox(_, _, _)).Times(1);

            this->message.Configure(123, "", {});
        }
    } // namespace Plugin
} // namespace UKControllerPluginTest

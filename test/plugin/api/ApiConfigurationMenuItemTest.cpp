#include "api/ApiConfigurationMenuItem.h"
#include "mock/MockWinApi.h"

using UKControllerPlugin::Api::ApiConfigurationMenuItem;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Api {

        class ApiConfigurationMenuItemTest : public Test
        {
            public:

                ApiConfigurationMenuItemTest()
                    : menuItem(mockWindows, 55)
                {

                }
                NiceMock<MockWinApi> mockWindows;
                ApiConfigurationMenuItem menuItem;
        };

        TEST_F(ApiConfigurationMenuItemTest, ItReturnsTheMenuItem)
        {
            PopupMenuItem expected;
            expected.firstValue = "Replace Personal API Configuration";
            expected.secondValue = "";
            expected.callbackFunctionId = 55;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.fixedPosition = false;

            EXPECT_TRUE(expected == this->menuItem.GetConfigurationMenuItem());
        }

        TEST_F(ApiConfigurationMenuItemTest, ConfigureStartsTheKeyReplacementProcedure)
        {
            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(
                    testing::StrEq(L"Please select the key file to use, this will overwrite your previous key."),
                    testing::StrEq(L"UKCP Message"),
                    MB_OKCANCEL | MB_ICONINFORMATION
                )
            )
                .Times(1)
                .WillOnce(Return(IDCANCEL));

            this->menuItem.Configure(55, "Test", {});
        }
    }  // namespace Api
}  // namespace UKControllerPluginTest

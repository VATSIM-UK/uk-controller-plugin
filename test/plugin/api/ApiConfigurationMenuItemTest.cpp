#include "api/ApiConfigurationMenuItem.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Api::ApiConfigurationMenuItem;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Windows::MockWinApi;
namespace UKControllerPluginTest::Api {

    class ApiConfigurationMenuItemTest : public ApiTestCase
    {
        public:
        ApiConfigurationMenuItemTest() : ApiTestCase(), menuItem(this->SettingsProvider(), mockWindows, 55)
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

    TEST_F(ApiConfigurationMenuItemTest, ConfigureDoesntReplaceTheSettingsIfUserDoesntReload)
    {
        EXPECT_CALL(this->SettingsProvider(), Reload).Times(1).WillOnce(testing::Return(false));

        this->ExpectNoApiRequests();

        this->menuItem.Configure(55, "Test", {});
    }

    TEST_F(ApiConfigurationMenuItemTest, ConfigureReplacesTheApiSettings)
    {
        EXPECT_CALL(this->SettingsProvider(), Reload).Times(1).WillOnce(testing::Return(true));

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnOk();

        EXPECT_CALL(
            this->mockWindows,
            OpenMessageBox(testing::_, testing::StrEq(L"Configuration Updated"), MB_OK | MB_ICONINFORMATION))
            .Times(1)
            .WillOnce(Return(IDOK));

        this->menuItem.Configure(55, "Test", {});
    }

    TEST_F(ApiConfigurationMenuItemTest, ConfigureHandlesAuthenticationErrorDuringAuthCheck)
    {
        EXPECT_CALL(this->SettingsProvider(), Reload).Times(1).WillOnce(testing::Return(true));

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnForbidden();

        EXPECT_CALL(
            this->mockWindows,
            OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Invalid"), MB_OK | MB_ICONWARNING))
            .Times(1)
            .WillOnce(Return(IDOK));

        this->menuItem.Configure(55, "Test", {});
    }

    TEST_F(ApiConfigurationMenuItemTest, ConfigureHandlesServerErrorDuringAuthCheck)
    {
        EXPECT_CALL(this->SettingsProvider(), Reload).Times(1).WillOnce(testing::Return(true));

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnServerError();

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::StrEq(L"Server Error"), testing::_))
            .Times(1)
            .WillOnce(Return(IDOK));

        this->menuItem.Configure(55, "Test", {});
    }
} // namespace UKControllerPluginTest::Api

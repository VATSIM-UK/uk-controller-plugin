#include "api/FirstTimeApiAuthorisationChecker.h"
#include "api/ApiRequestData.h"
#include "api/ApiRequestException.h"
#include "api/Response.h"
#include "dialog/DialogManager.h"

using UKControllerPlugin::Api::FirstTimeApiAuthorisationCheck;
using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiRequestException;
using UKControllerPluginUtils::Api::Response;

namespace UKControllerPluginTest::Api {
    class FirstTimeApiAuthorisationCheckerTest : public ApiTestCase
    {
        public:
        FirstTimeApiAuthorisationCheckerTest() : dialogManager(dialogProvider)
        {
            dialogManager.AddDialog(dialogDataRequest);
        }

        UKControllerPlugin::Dialog::DialogData dialogDataRequest = {IDD_API_KEY_REPLACE, "", nullptr, NULL, nullptr};
        testing::NiceMock<Dialog::MockDialogProvider> dialogProvider;
        UKControllerPlugin::Dialog::DialogManager dialogManager;
        testing::NiceMock<Windows::MockWinApi> windows;
        testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiSettingsProvider> settingsProvider;
    };

    TEST_F(FirstTimeApiAuthorisationCheckerTest, ItPerformsASuccessfulCheck)
    {
        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnOk();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows, dialogManager);
    }

    TEST_F(FirstTimeApiAuthorisationCheckerTest, ItHandlesAServerErrorDuringCheck)
    {
        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Server Error"), testing::_)).Times(1);

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnServerError();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows, dialogManager);
    }

    TEST_F(FirstTimeApiAuthorisationCheckerTest, ItRetriesTheCheckIfTheUserReplacesConfig)
    {
        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Invalid"), testing::_))
            .Times(1)
            .WillOnce(testing::Return(IDOK));

        EXPECT_CALL(dialogProvider, OpenDialog(this->dialogDataRequest, testing::_))
            .Times(1)
            .WillOnce(testing::Invoke([](auto dialog, auto arg) {
                bool* dataReceived = reinterpret_cast<bool*>(
                    reinterpret_cast<const UKControllerPlugin::Dialog::DialogCallArgument*>(arg)->contextArgument);
                *dataReceived = true;
            }));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Not Updated"), testing::_))
            .Times(0);

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnOk();

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnForbidden();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows, dialogManager);
    }

    TEST_F(
        FirstTimeApiAuthorisationCheckerTest,
        ItDoesntRetryIfUserChoosesNotToReplaceConfigAtFirstPromptAfterAuthorisationFailure)
    {
        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Invalid"), testing::_))
            .Times(1)
            .WillOnce(testing::Return(IDCANCEL));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Not Updated"), testing::_))
            .Times(1);

        EXPECT_CALL(dialogProvider, OpenDialog(this->dialogDataRequest, testing::_)).Times(0);

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnForbidden();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows, dialogManager);
    }

    TEST_F(
        FirstTimeApiAuthorisationCheckerTest,
        ItDoesntRetryIfUserChoosesNotToReplaceConfigAtReplaceDialogAfterAuthorisationFailure)
    {
        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Invalid"), testing::_))
            .Times(1)
            .WillOnce(testing::Return(IDOK));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Not Updated"), testing::_))
            .Times(1);

        EXPECT_CALL(dialogProvider, OpenDialog(this->dialogDataRequest, testing::_))
            .Times(1)
            .WillOnce(testing::Invoke([](auto dialog, auto arg) {
                bool* dataReceived = reinterpret_cast<bool*>(
                    reinterpret_cast<const UKControllerPlugin::Dialog::DialogCallArgument*>(arg)->contextArgument);
                *dataReceived = false;
            }));

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnForbidden();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows, dialogManager);
    }
} // namespace UKControllerPluginTest::Api

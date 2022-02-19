#include "api/FirstTimeApiAuthorisationChecker.h"
#include "api/ApiRequestData.h"
#include "api/ApiRequestException.h"
#include "api/Response.h"

using UKControllerPlugin::Api::FirstTimeApiAuthorisationCheck;
using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiRequestException;
using UKControllerPluginUtils::Api::Response;

namespace UKControllerPluginTest::Api {
    class FirstTimeApiAuthorisationCheckerTest : public ApiTestCase
    {
        public:
        testing::NiceMock<Windows::MockWinApi> windows;
    };

    TEST_F(FirstTimeApiAuthorisationCheckerTest, ItPerformsASuccessfulCheck)
    {
        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnOk();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows);
    }

    TEST_F(FirstTimeApiAuthorisationCheckerTest, ItHandlesAServerErrorDuringCheck)
    {
        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Server Error"), testing::_)).Times(1);

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnServerError();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows);
    }

    TEST_F(FirstTimeApiAuthorisationCheckerTest, ItRetriesTheCheckIfTheUserReplacesConfig)
    {
        EXPECT_CALL(this->SettingsProvider(), Reload).Times(1).WillOnce(testing::Return(true));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Invalid"), testing::_))
            .Times(1)
            .WillOnce(testing::Return(IDOK));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Not Updated"), testing::_))
            .Times(0);

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnOk();

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnForbidden();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows);
    }

    TEST_F(FirstTimeApiAuthorisationCheckerTest, ItDoesntRetryIfUserChoosesNotToReplaceConfigAfterAuthorisationFailure)
    {
        EXPECT_CALL(this->SettingsProvider(), Reload).Times(0);

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Invalid"), testing::_))
            .Times(1)
            .WillOnce(testing::Return(IDCANCEL));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Not Updated"), testing::_))
            .Times(1);

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnForbidden();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows);
    }

    TEST_F(FirstTimeApiAuthorisationCheckerTest, ItDoesntRetryIfConfigReloadDoesntHappenAfterAuthorisationFailure)
    {
        EXPECT_CALL(this->SettingsProvider(), Reload).Times(1).WillOnce(testing::Return(false));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Invalid"), testing::_))
            .Times(1)
            .WillOnce(testing::Return(IDOK));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::StrEq(L"UKCP API Config Not Updated"), testing::_))
            .Times(1);

        this->ExpectApiRequest()->Get().To("authorise").WithoutBody().WillReturnForbidden();

        FirstTimeApiAuthorisationCheck(this->SettingsProvider(), windows);
    }
} // namespace UKControllerPluginTest::Api

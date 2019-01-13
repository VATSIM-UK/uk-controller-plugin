#include "pch/pch.h"
#include "update/PluginUpdateChecker.h"
#include "helper/Matchers.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "api/ApiException.h"

using UKControllerPlugin::Update::PluginUpdateChecker;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::ApiException;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::Throw;

namespace UKControllerPluginTest {
namespace Update {

class PluginUpdateCheckerTest : public Test
{
    public:
        PluginUpdateCheckerTest()
        {

        }

        NiceMock <MockApiInterface> helper;
        NiceMock<MockWinApi> winApiMock;
};

TEST_F(PluginUpdateCheckerTest, CheckForUpdatesReturnsNormalIfNoUpdates)
{
    EXPECT_CALL(helper, UpdateCheck("1.0"))
        .Times(1)
        .WillOnce(Return(MockApiInterface::UPDATE_UP_TO_DATE));

    EXPECT_EQ(
        PluginUpdateChecker::versionAllowed,
        PluginUpdateChecker::CheckForUpdates("1.0", this->winApiMock, this->helper)
    );
}

TEST_F(PluginUpdateCheckerTest, CheckForUpdatesShowsMessageIfResponseNotOk)
{
    EXPECT_CALL(helper, UpdateCheck("1.0"))
        .Times(1)
        .WillOnce(Throw(ApiException("no worky")));

    EXPECT_CALL(
        winApiMock,
        OpenMessageBox(
            StrEq(L"Unable to check plugin version, remote functionality disabled"),
            StrEq(L"UKCP Error"),
            MB_OK | MB_ICONWARNING
        ))
        .Times(1);

    EXPECT_EQ(
        PluginUpdateChecker::unsupportedVersion,
        PluginUpdateChecker::CheckForUpdates("1.0", this->winApiMock, this->helper)
    );
}

TEST_F(PluginUpdateCheckerTest, CheckForUpdatesShowsMessageIfOutOfDate)
{
    EXPECT_CALL(helper, UpdateCheck("1.0"))
        .Times(1)
        .WillOnce(Return(MockApiInterface::UPDATE_VERSION_NEEDS_UPDATE));

    EXPECT_CALL(
        winApiMock,
        OpenMessageBox(
            StrEq(L"An update to the plugin is available."),
            StrEq(L"UKCP Update Notification"),
            MB_OK | MB_ICONINFORMATION
        ))
        .Times(1);

    EXPECT_EQ(
        PluginUpdateChecker::versionAllowed,
        PluginUpdateChecker::CheckForUpdates("1.0", this->winApiMock, this->helper)
    );
}

TEST_F(PluginUpdateCheckerTest, CheckForUpdatesShowsMessageIfVersionDeprecated)
{
    EXPECT_CALL(helper, UpdateCheck("1.0"))
        .Times(1)
        .WillOnce(Return(MockApiInterface::UPDATE_VERSION_DISABLED));

    EXPECT_CALL(
            winApiMock,
            OpenMessageBox(
                StrEq(
                    L"This version of the plugin has been withdrawn, you need to update to continue using the plugin."),
                    StrEq(L"UKCP Update Notification"),
                    MB_OK | MB_ICONERROR
                ))
        .Times(1);

    EXPECT_EQ(
        PluginUpdateChecker::unsupportedVersion,
        PluginUpdateChecker::CheckForUpdates("1.0", this->winApiMock, this->helper)
    );
}
}  // namespace Update
}  // namespace UKControllerPluginTest

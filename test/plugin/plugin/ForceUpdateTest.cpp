#include "pch/pch.h"
#include "plugin/ForceUpdate.h"
#include "plugin/PopupMenuItem.h"
#include "mock/MockWinApi.h"

using UKControllerPlugin::Plugin::ForceUpdate;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Windows::MockWinApi;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Plugin {

        class ForceUpdateTest : public Test
        {
            public:

                ForceUpdateTest()
                    : forceUpdate(mockWindows, 123)
                {

                }

                NiceMock<MockWinApi> mockWindows;
                ForceUpdate forceUpdate;
        };

        TEST_F(ForceUpdateTest, ItHasAConfigurationMenuItem)
        {
            PopupMenuItem expected;
            expected.firstValue = "Force Plugin To Update";
            expected.secondValue = "";
            expected.callbackFunctionId = 123;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.fixedPosition = false;
            EXPECT_EQ(expected, this->forceUpdate.GetConfigurationMenuItem());
        }

        TEST_F(ForceUpdateTest, ProcessCommandReturnsFalseIfNotCommand)
        {
            EXPECT_FALSE(this->forceUpdate.ProcessCommand(".ukcp notthis"));
        }

        TEST_F(ForceUpdateTest, ProcessCommandReturnsTrueIfCommand)
        {
            EXPECT_TRUE(this->forceUpdate.ProcessCommand(".ukcp forceupdate"));
        }

        TEST_F(ForceUpdateTest, ProcessCommandShowsMessage)
        {
            EXPECT_CALL(this->mockWindows, OpenMessageBox(::testing::_, ::testing::_, MB_YESNO | MB_ICONEXCLAMATION))
                .Times(1)
                .WillOnce(testing::Return(IDNO));

            this->forceUpdate.ProcessCommand(".ukcp forceupdate");
        }

        TEST_F(ForceUpdateTest, ConfigureShowsMessage)
        {
            EXPECT_CALL(this->mockWindows, OpenMessageBox(::testing::_, ::testing::_, MB_YESNO | MB_ICONEXCLAMATION))
                .Times(1)
                .WillOnce(testing::Return(IDNO));

            this->forceUpdate.Configure(123, "", {});
        }

        TEST_F(ForceUpdateTest, FilesAreMovedIfUserSelectsYes)
        {
            EXPECT_CALL(this->mockWindows, OpenMessageBox(::testing::_, ::testing::_, MB_YESNO | MB_ICONEXCLAMATION))
                .Times(1)
            .WillOnce(testing::Return(IDYES));

            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
            .Times(1)
            .WillOnce(testing::Return(true));

            EXPECT_CALL(
                this->mockWindows,
                MoveFileToNewLocation(
                    std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                    std::wstring(L"bin/UKControllerPluginUpdater.dll.old")
                )
            )
                .Times(1);

            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .Times(1)
                .WillOnce(testing::Return(true));

            EXPECT_CALL(
                this->mockWindows,
                MoveFileToNewLocation(
                    std::wstring(L"bin/UKControllerPluginCore.dll"),
                    std::wstring(L"bin/UKControllerPluginCore.dll.old")
                )
            )
                .Times(1);

            EXPECT_CALL(this->mockWindows, OpenMessageBox(::testing::_, ::testing::_, MB_OK | MB_ICONINFORMATION))
                .Times(1)
                .WillOnce(testing::Return(IDOK));

            this->forceUpdate.Configure(123, "", {});
        }

        TEST_F(ForceUpdateTest, FilesAreNotMovedIfNotPresentToBeMoved)
        {
            EXPECT_CALL(this->mockWindows, OpenMessageBox(::testing::_, ::testing::_, MB_YESNO | MB_ICONEXCLAMATION))
                .Times(1)
                .WillOnce(testing::Return(IDYES));

            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
                .WillOnce(testing::Return(false));

            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .Times(1)
                .WillOnce(testing::Return(false));

            EXPECT_CALL(
                this->mockWindows,
                MoveFileToNewLocation(
                    testing::_,
                    testing::_
                )
            )
                .Times(0);

            EXPECT_CALL(this->mockWindows, OpenMessageBox(::testing::_, ::testing::_, MB_OK | MB_ICONINFORMATION))
                .Times(1)
                .WillOnce(testing::Return(IDOK));

            this->forceUpdate.Configure(123, "", {});
        }
    }  // namespace Plugin
}  // namespace UKControllerPluginTest

#include "api/FirstTimeApiConfigLoader.h"
#include "dialog/DialogManager.h"

namespace UKControllerPluginTest::Api {

    class FirstTimeApiConfigLoaderTest : public testing::Test
    {
        public:
        FirstTimeApiConfigLoaderTest() : dialogManager(dialogProvider)
        {
            dialogManager.AddDialog(dialogDataRequest);
        }

        UKControllerPlugin::Dialog::DialogData dialogDataRequest = {IDD_API_KEY_REPLACE, "", nullptr, NULL, nullptr};
        testing::NiceMock<Dialog::MockDialogProvider> dialogProvider;
        UKControllerPlugin::Dialog::DialogManager dialogManager;
        testing::NiceMock<Windows::MockWinApi> mockWindows;
        testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiSettingsProvider> settingsProvider;
    };

    TEST_F(FirstTimeApiConfigLoaderTest, ItReturnsTrueIfConfigLocated)
    {
        EXPECT_CALL(settingsProvider, Has).Times(1).WillOnce(testing::Return(true));

        EXPECT_CALL(dialogProvider, OpenDialog).Times(0);

        EXPECT_TRUE(UKControllerPlugin::Api::LocateConfig(dialogManager, mockWindows, settingsProvider));
    }

    TEST_F(FirstTimeApiConfigLoaderTest, ItReturnsTrueIfUserReplacesConfig)
    {
        EXPECT_CALL(settingsProvider, Has).Times(1).WillOnce(testing::Return(false));
        EXPECT_CALL(mockWindows, OpenMessageBox(testing::_, testing::_, MB_OK)).Times(1);
        EXPECT_CALL(dialogProvider, OpenDialog(this->dialogDataRequest, testing::_))
            .Times(1)
            .WillOnce(testing::Invoke([](auto dialog, auto arg) {
                bool* dataReceived = reinterpret_cast<bool*>(
                    reinterpret_cast<const UKControllerPlugin::Dialog::DialogCallArgument*>(arg)->contextArgument);
                *dataReceived = true;
            }));

        EXPECT_TRUE(UKControllerPlugin::Api::LocateConfig(dialogManager, mockWindows, settingsProvider));
    }

    TEST_F(FirstTimeApiConfigLoaderTest, ItReturnsFalseIfUserDoesNotReplaceConfig)
    {
        EXPECT_CALL(settingsProvider, Has).Times(1).WillOnce(testing::Return(false));
        EXPECT_CALL(mockWindows, OpenMessageBox(testing::_, testing::_, MB_OK)).Times(1);
        EXPECT_CALL(dialogProvider, OpenDialog(this->dialogDataRequest, testing::_))
            .Times(1)
            .WillOnce(testing::Invoke([](auto dialog, auto arg) {
                bool* dataReceived = reinterpret_cast<bool*>(
                    reinterpret_cast<const UKControllerPlugin::Dialog::DialogCallArgument*>(arg)->contextArgument);
                *dataReceived = false;
            }));

        EXPECT_FALSE(UKControllerPlugin::Api::LocateConfig(dialogManager, mockWindows, settingsProvider));
    }
} // namespace UKControllerPluginTest::Api

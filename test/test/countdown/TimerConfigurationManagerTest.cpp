#include "pch/pch.h"
#include "countdown/TimerConfiguration.h"
#include "countdown/TimerConfigurationManager.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "euroscope/UserSetting.h"
#include "plugin/PopupMenuitem.h"
#include "dialog/DialogData.h"

using UKControllerPlugin::Countdown::TimerConfiguration;
using UKControllerPlugin::Countdown::TimerConfigurationManager;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Dialog::DialogData;
using testing::Test;
using testing::NiceMock;
using testing::_;

namespace UKControllerPluginTest {
    namespace Countdown {

        class TimerConfigurationManagerTest : public Test
        {
            public:
                TimerConfigurationManagerTest()
                    : dialogManager(mockDialogProvider), userSettings(mockUserSettingProvider),
                    manager(userSettings, dialogManager, 1)
                {
                    this->dialogManager.AddDialog(configurationDialog);
                }

                TimerConfiguration config1 = { 1, true, 120 };
                TimerConfiguration config2 = { 2, false, 140 };
                TimerConfiguration config3 = { 3, true, 180 };
                DialogData configurationDialog = { IDD_TIMER_CONFIGURATION, "", NULL, NULL };

                NiceMock<MockDialogProvider> mockDialogProvider;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                UserSetting userSettings;
                DialogManager dialogManager;
                TimerConfigurationManager manager;
        };

        TEST_F(TimerConfigurationManagerTest, ItStartsWithNoTimers)
        {
            EXPECT_EQ(0, this->manager.CountTimers());
        }

        TEST_F(TimerConfigurationManagerTest, GetTimerReturnsInvalidTimerIfNotFound)
        {
            EXPECT_EQ(this->manager.invalidTimer, this->manager.GetTimer(1));
        }

        TEST_F(TimerConfigurationManagerTest, ItAddsTimers)
        {
            this->manager.AddTimer(config1);
            this->manager.AddTimer(config2);
            EXPECT_EQ(2, this->manager.CountTimers());

            EXPECT_EQ(config1, this->manager.GetTimer(1));
        }

        TEST_F(TimerConfigurationManagerTest, ItUpdatesTimersOnDuplicates)
        {
            this->manager.AddTimer(config1);
            TimerConfiguration newConfig1 = { 1, true, 999 };
            this->manager.AddTimer(newConfig1);

            TimerConfiguration actualConfig = this->manager.GetTimer(1);
            EXPECT_EQ(newConfig1.timerId, actualConfig.timerId);
            EXPECT_EQ(newConfig1.timerDuration, actualConfig.timerDuration);
            EXPECT_EQ(newConfig1.timerEnabled, actualConfig.timerEnabled);

            EXPECT_EQ(1, this->manager.CountTimers());
        }

        TEST_F(TimerConfigurationManagerTest, ItCountsEnabledTimers)
        {
            this->manager.AddTimer(config1);
            this->manager.AddTimer(config2);
            this->manager.AddTimer(config3);

            EXPECT_EQ(2, this->manager.CountEnabledTimers());
        }

        TEST_F(TimerConfigurationManagerTest, ItReturnsAConfigurationMenuItem)
        {
            PopupMenuItem expected;
            expected.firstValue = this->manager.menuItemDescription;
            expected.secondValue = "";
            expected.callbackFunctionId = 1;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.fixedPosition = false;

            EXPECT_EQ(expected, this->manager.GetConfigurationMenuItem());
        }

        TEST_F(TimerConfigurationManagerTest, ItCallsTheDialogManager)
        {
            EXPECT_CALL(this->mockDialogProvider, OpenDialog(this->configurationDialog, _))
                .Times(1);


            this->manager.Configure(1, "", {});
        }
    }  // namespace Countdown
}  // namespace UKControllerPluginTest

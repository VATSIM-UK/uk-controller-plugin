#pragma once
#include "euroscope/UserSettingAwareCollection.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            A class for handling the dialog that configures the countdown timer.
        */
        class TimerConfigurationDialog
        {
            public:
                TimerConfigurationDialog(
                    UKControllerPlugin::Euroscope::UserSetting & userSettings,
                    const UKControllerPlugin::Euroscope::UserSettingAwareCollection & userSettingHandlers
                );
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void InitDialog(HWND hwnd, LPARAM lParam);
                void SaveDialog(HWND hwnd);
                void InitTimerRow(
                    HWND hwnd,
                    unsigned int timerId,
                    unsigned int enabledCheckboxId,
                    unsigned int durationBoxId
                );
                void SaveTimerRow(
                    HWND hwnd,
                    unsigned int timerId,
                    unsigned int enabledCheckboxId,
                    unsigned int durationBoxId
                );

                // Handles the user settings
                UKControllerPlugin::Euroscope::UserSetting & userSettings;

                // Handles the user settings updated event
                const UKControllerPlugin::Euroscope::UserSettingAwareCollection & userSettingHandlers;
        };
    }  // namespace Countdown
}  // namespace UKControllerPlugin

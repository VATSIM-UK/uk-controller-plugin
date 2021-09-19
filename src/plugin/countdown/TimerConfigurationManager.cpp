#include "GlobalCountdownSettingFunctions.h"
#include "TimerConfigurationManager.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin {
    namespace Countdown {

        TimerConfigurationManager::TimerConfigurationManager(
            const DialogManager& dialogManager, const unsigned int menuCallbackFunctionId)
            : menuCallbackFunctionId(menuCallbackFunctionId), dialogManager(dialogManager)
        {
        }

        /*
            Replaces the specified timer
        */
        void TimerConfigurationManager::AddTimer(TimerConfiguration timer)
        {
            this->timers.erase(timer);
            this->timers.insert(timer);
            this->configVersion++;
        }

        /*
            Returns the number of enabled timers
        */
        auto TimerConfigurationManager::CountEnabledTimers() const -> size_t
        {
            size_t numberEnabled = 0;
            for (auto it = this->timers.cbegin(); it != this->timers.cend(); ++it) {
                if (it->timerEnabled) {
                    numberEnabled++;
                }
            }

            return numberEnabled;
        }

        /*
            Returns number of timers
        */
        auto TimerConfigurationManager::CountTimers() const -> size_t
        {
            return this->timers.size();
        }

        /*
            Returns whether the config has been updated since the given time
        */
        auto TimerConfigurationManager::GetConfigVersion() const -> unsigned int
        {
            return this->configVersion;
        }

        /*
            Returns the given timer
        */
        auto TimerConfigurationManager::GetTimer(unsigned int timerId) const -> TimerConfiguration
        {
            auto timer = std::find_if(
                this->timers.cbegin(), this->timers.cend(), [timerId](const TimerConfiguration& storedTimer) -> bool {
                    return timerId == storedTimer.timerId;
                });

            return timer != this->timers.cend() ? *timer : this->invalidTimer;
        }

        /*
            Run the configuration routine - open the configuration dialog
        */
        void TimerConfigurationManager::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->dialogManager.OpenDialog(IDD_TIMER_CONFIGURATION, NULL);
        }

        /*
            Return the menu item we want to display
        */
        auto TimerConfigurationManager::GetConfigurationMenuItem() const -> PopupMenuItem
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->menuItemDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->menuCallbackFunctionId;
            returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        /*
            Update each of the timers based on the user settings
        */
        void TimerConfigurationManager::UserSettingsUpdated(UKControllerPlugin::Euroscope::UserSetting& userSettings)
        {
            for (auto it = this->timers.begin(); it != this->timers.end();) {
                TimerConfiguration config = {
                    it->timerId,
                    userSettings.GetBooleanEntry(GetTimerEnabledKey(*it), false),
                    userSettings.GetUnsignedIntegerEntry(GetTimerDurationKey(*it), 60)};

                it = this->timers.erase(it);
                this->timers.insert(config);
            }

            this->configVersion++;
        }
    } // namespace Countdown
} // namespace UKControllerPlugin

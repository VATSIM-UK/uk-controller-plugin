#pragma once
#include "euroscope/UserSetting.h"
#include "countdown/TimerConfiguration.h"
#include "dialog/DialogManager.h"
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            A class for managing timer configurations
            and providing useful functions for querying them.
        */
        class TimerConfigurationManager : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
        {
            public:

                TimerConfigurationManager(
                    UKControllerPlugin::Euroscope::UserSetting & settingProvider,
                    const UKControllerPlugin::Dialog::DialogManager & dialogManager,
                    const unsigned int menuCallbackFunctionId
                );
                void AddTimer(UKControllerPlugin::Countdown::TimerConfiguration timer);
                size_t CountEnabledTimers(void) const;
                size_t CountTimers(void) const;
                UKControllerPlugin::Countdown::TimerConfiguration GetTimer(int timerId) const;

                typedef std::set<UKControllerPlugin::Countdown::TimerConfiguration> AllTimers;
                typedef AllTimers::const_iterator const_iterator;
                const_iterator cbegin() const { return timers.cbegin(); }
                const_iterator cend() const { return timers.cend(); }

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                const std::string menuItemDescription = "Configure Countdown Timer";

                const unsigned int menuCallbackFunctionId;

                // The invalid timer
                const UKControllerPlugin::Countdown::TimerConfiguration invalidTimer = { 0, false, 0 };

            private:
                
                // Allows timer settings to be saved
                UKControllerPlugin::Euroscope::UserSetting & settingProvider;
                
                // Lets you open dialogs
                const UKControllerPlugin::Dialog::DialogManager & dialogManager;

                // Contains all the timers
                std::set<UKControllerPlugin::Countdown::TimerConfiguration> timers;
        };
    }  // namespace Countdown
}  // namespace UKControllerPlugin

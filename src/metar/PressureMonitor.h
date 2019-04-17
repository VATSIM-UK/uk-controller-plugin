#pragma once
#include "metar/MetarEventHandlerInterface.h"
#include "euroscope/UserSetting.h"
#include "message/UserMessager.h"
#include "euroscope/UserSettingAwareInterface.h"

namespace UKControllerPlugin {
    namespace Metar {

        /*
            Monitors changing QNHs as reported by EuroScope
            and notifies the user if the QNH has changed.
        */
        class PressureMonitor : public UKControllerPlugin::Metar::MetarEventHandlerInterface,
            public UKControllerPlugin::Euroscope::UserSettingAwareInterface
        {
            public:

                explicit PressureMonitor(
                    UKControllerPlugin::Message::UserMessager & userMessager
                );
                std::string GetStoredQnh(std::string station) const;
                bool NotificationsEnabled(void) const;
                void SetNotficationsEnabled(bool enabled);

                // Inherited via MetarEventHandlerInterface
                void NewMetar(std::string station, std::string metar) override;

                // Inherited via UserSettingAwareInterface
                void UserSettingsUpdated(UKControllerPlugin::Euroscope::UserSetting & userSettings) override;

                // String to return if no QNHn is stored
                const std::string qnhNotStored = "NONE";

            private:
                
                // Whether or not to send notifications.
                bool notificationsEnabled = false;
                
                // A map of airfield -> last recorded QNH.
                std::map<std::string, std::string> qnhs;
                
                // Interface with ES for sending messages to the user about pressure changes.
                UKControllerPlugin::Message::UserMessager & userMessager;
        };
    }  // namespace Metar
}  // namespace UKControllerPlugin

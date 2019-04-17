#pragma once
#include "metar/MetarEventHandlerInterface.h"
#include "euroscope/UserSetting.h"
#include "message/UserMessager.h"

namespace UKControllerPlugin {
    namespace Metar {

        /*
            Monitors changing QNHs as reported by EuroScope
            and notifies the user if the QNH has changed.
        */
        class PressureMonitor : public MetarEventHandlerInterface
        {
            public:

                PressureMonitor(
                    UKControllerPlugin::Euroscope::UserSetting & pluginUserSettings,
                    UKControllerPlugin::Message::UserMessager & userMessager
                );
                std::string GetStoredQnh(std::string station) const;
                // Inherited via MetarEventHandlerInterface
                void NewMetar(std::string station, std::string metar) override;

                // String to return if no QNHn is stored
                const std::string qnhNotStored = "NONE";

            private:
                
                // A map of airfield -> last recorded QNH.
                std::map<std::string, std::string> qnhs;

                // A place for retrieving and saving user settings
                UKControllerPlugin::Euroscope::UserSetting & pluginUserSettings;
                
                // Interface with ES for sending messages to the user about pressure changes.
                UKControllerPlugin::Message::UserMessager & userMessager;
        };
    }  // namespace Metar
}  // namespace UKControllerPlugin

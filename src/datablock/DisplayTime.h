#pragma once
#include "euroscope/UserSettingAwareInterface.h"

namespace UKControllerPlugin {
    namespace Datablock {

        /*
            A class for converting times to string representations, providing
            a wrapper for Howard Hinnant's date library. Also provides format
            for "missing times" - blank or --:--.
        */
        class DisplayTime : public UKControllerPlugin::Euroscope::UserSettingAwareInterface
        {
            public:
                std::string FromTimestamp(time_t time) const;
                std::string FromSystemTime(void) const;
                std::string FromTimePoint(std::chrono::system_clock::time_point tp) const;
                inline std::string GetUnknownTimeFormat(void) const
                {
                    return this->useBlankTimeForUnknown
                        ? this->unknownTimeFormatBlank
                        : this->unknownTimeFormatDefault;
                };

                // Inherited via UserSettingAwareInterface
                void UserSettingsUpdated(UKControllerPlugin::Euroscope::UserSetting & userSettings) override;

                // The format for time strings
                const std::string timeFormat = "%H:%M";

                // The default format to use when times are unknown
                const std::string unknownTimeFormatDefault = "--:--";

                // The format to use for unknown times when the user has specifically stated that they want it blank
                const std::string unknownTimeFormatBlank = "";

            private:

                // Whether or not blank times should be used for unknown times.
                bool useBlankTimeForUnknown = false;
        };
    }  // namespace Datablock
}  // namespace UKControllerPlugin

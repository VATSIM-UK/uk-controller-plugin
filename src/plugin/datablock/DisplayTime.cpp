#include "datablock/DisplayTime.h"
#include "euroscope/UserSetting.h"
#include "euroscope/GeneralSettingsEntries.h"

using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Datablock {

        /*
            Converts a timestamp to its Zulu string representation for display.
        */
        std::string DisplayTime::FromTimestamp(time_t time) const
        {
            return fmt::format("{:%H:%M}", fmt::gmtime(std::chrono::system_clock::from_time_t(time)));
        }

        /*
            Converts the current system time in UTC to its zulu string representation
            for display.
        */
        std::string DisplayTime::FromSystemTime(void) const
        {
            return fmt::format("{:%H:%M}", fmt::gmtime(std::chrono::system_clock::now()));
        }

        /*
            Converts the current system time in UTC to its zulu string representation
            for display.
        */
        std::string DisplayTime::FromTimePoint(std::chrono::system_clock::time_point tp) const
        {
            return fmt::format("{:%H:%M}", fmt::gmtime(tp));
        }

        /*
            Handle the fact that user settings have been updated
        */
        void DisplayTime::UserSettingsUpdated(UserSetting& userSettings)
        {
            this->useBlankTimeForUnknown =
                userSettings.GetBooleanEntry(GeneralSettingsEntries::unknownTimeFormatBlankKey, false);
        }
    } // namespace Datablock
} // namespace UKControllerPlugin

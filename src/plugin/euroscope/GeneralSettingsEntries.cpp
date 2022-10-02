#include "euroscope/GeneralSettingsEntries.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        // PRENOTES
        const std::string GeneralSettingsEntries::usePrenoteSettingsKey = "prenoteNotify";
        const std::string GeneralSettingsEntries::usePrenoteSettingsDescription =
            "Notify the user of required controller pre-notes";

        // SQUAWKS
        const std::string GeneralSettingsEntries::squawkToggleSettingsKey = "autoAssignSquawks";
        const std::string GeneralSettingsEntries::squawkToggleSettingsDescription = "Automatically assign squawks";

        // Initial Altitudes
        const std::string GeneralSettingsEntries::initialAltitudeToggleSettingsKey = "autoAssignInitialAltitudes";
        const std::string GeneralSettingsEntries::initialAltitudeToggleSettingsDescription =
            "Automatically assign initial altitudes";

        // PRESSURE MONITOR
        const std::string GeneralSettingsEntries::pressureMonitorSendMessageKey = "notifyPressureChanges";
        const std::string GeneralSettingsEntries::pressureMonitorSendMessageDescription =
            "Notify On Pressure Changes At Airports";

        // TIME FORMAT
        const std::string GeneralSettingsEntries::unknownTimeFormatBlankKey = "useBlankForUnknownTimes";
        const std::string GeneralSettingsEntries::unknownTimeFormatBlankDescription = "Display Unknown Times As Blank";

        // Initial Headings
        const std::string GeneralSettingsEntries::initialHeadingToggleSettingsKey = "autoAssignInitialHeadings";
        const std::string GeneralSettingsEntries::initialHeadingToggleSettingsDescription =
            "Automatically assign initial headings";
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

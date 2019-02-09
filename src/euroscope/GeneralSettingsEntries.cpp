#include "pch/stdafx.h"
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
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

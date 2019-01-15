#include "pch/stdafx.h"
#include "euroscope/GeneralSettingsEntries.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        // PRENOTES
        const std::string GeneralSettingsEntries::usePrenoteSettingsKey = "prenoteNotify";
        const std::string GeneralSettingsEntries::usePrenoteSettingsDescription =
            "Notify the user of required controller pre-notes";

        // SQUAWKS
        const std::string squawkToggleSettingsKey = "autoAssignSquawks";
        const std::string squawkToggleSettingsDescription = "Automatically assign squawks";

        // Initial Altitudes
        const std::string initialAltitudeToggleSettingsKey = "autoAssignInitialAltitudes";
        const std::string initialAltitudeToggleSettingsDescription = "Automatically assign initial altitudes";
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

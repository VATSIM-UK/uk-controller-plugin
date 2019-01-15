#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Entries into the general settings file.
        */
        typedef struct GeneralSettingsEntries
        {
            // PRENOTES
            static const std::string usePrenoteSettingsKey;
            static const std::string usePrenoteSettingsDescription;

            // SQUAWKS
            static const std::string squawkToggleSettingsKey;
            static const std::string squawkToggleSettingsDescription;

            // Initial Altitudes
            static const std::string initialAltitudeToggleSettingsKey;
            static const std::string initialAltitudeToggleSettingsDescription;
        } GeneralSettingsEntries;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

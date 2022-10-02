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

            // PRESSURE MONITOR
            static const std::string pressureMonitorSendMessageKey;
            static const std::string pressureMonitorSendMessageDescription;

            // TIME FORMAT
            static const std::string unknownTimeFormatBlankKey;
            static const std::string unknownTimeFormatBlankDescription;

            // Initial Headings
            static const std::string initialHeadingToggleSettingsKey;
            static const std::string initialHeadingToggleSettingsDescription;
        } GeneralSettingsEntries;
    } // namespace Euroscope
} // namespace UKControllerPlugin

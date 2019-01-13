#pragma once

namespace UKControllerPlugin {
    namespace InitialAltitude {
        struct InitialAltitudeData;
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace InitialAltitude {

        /*
            Used for comparing InitialAltitude structs so that they can
            be accessed by string values.
        */
        typedef struct CompareInitialAltitudes
        {
            using is_transparent = std::string;

            bool operator()(
                const UKControllerPlugin::InitialAltitude::InitialAltitudeData & initial,
                std::string id
            ) const;
            bool operator()(
                std::string id,
                const UKControllerPlugin::InitialAltitude::InitialAltitudeData & initial
            ) const;
            bool operator()(
                const UKControllerPlugin::InitialAltitude::InitialAltitudeData & a,
                const UKControllerPlugin::InitialAltitude::InitialAltitudeData & b
            ) const;

        } CompareInitialAltitudes;
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin

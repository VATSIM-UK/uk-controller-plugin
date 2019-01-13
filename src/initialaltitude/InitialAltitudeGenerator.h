#pragma once
#include "initialaltitude/InitialAltitudeData.h"
#include "initialaltitude/CompareInitialAltitudes.h"

namespace UKControllerPlugin {
    namespace InitialAltitude {

        /*
            Class for generating an initial altitude.
        */
        class InitialAltitudeGenerator
        {
            public:
                void AddSid(std::string airfield, std::string sidName, int altitude);
                int Count(void) const;
                int GetInitialAltitudeForDeparture(std::string origin, std::string sid) const;
                bool HasSid(std::string origin, std::string sid) const;

            private:
                std::string GetSidIdentifier(std::string origin, std::string sid) const;

                // Set of initial altitudes
                std::set<
                    UKControllerPlugin::InitialAltitude::InitialAltitudeData,
                    UKControllerPlugin::InitialAltitude::CompareInitialAltitudes
                > altitudes;
        };
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin

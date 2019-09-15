#pragma once

namespace UKControllerPlugin {
    namespace Airfield {

        /*
            Represents a runway as defined in the sectorfile
        */
        class Runway
        {
            public:

                Runway(
                    std::string sectorfileIdentifier,
                    std::string airfield,
                    std::string identifier,
                    unsigned int heading
                );

                bool Active(void) const;
                bool ActiveForDepartures(void) const;
                bool ActiveForArrivals(void) const;
                void SetActiveForDepartures(bool active);
                void SetActiveForArrivals(bool active);

                // The runway identifier in the sector file
                const std::string sectorfileIdentifier;

                // The airfield associated with the runway
                const std::string airfield;

                // The regular identifier used for the runway
                const std::string identifier;

                // The runway heading, in degrees
                const unsigned int heading;

            private:

                // Is the runway active for departures
                bool activeForDepartures = false;

                // Is the runway active for arrivals
                bool activeForArrivals = false;
                
        };

    }  // namespace Airfield
}  // namespace UKControllerPlugin

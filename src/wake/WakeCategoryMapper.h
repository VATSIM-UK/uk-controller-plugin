#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Wake {

        /*
            Class that maps aircraft types to their
            UK wake categories.
        */
        class WakeCategoryMapper
        {
            public:
                void AddTypeMapping(std::string type, std::string category);
                int Count(void) const;
                std::string MapFlightplanToCategory(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan
                ) const;

            private:
                // Maps ICAO code to wake category
                std::map<std::string, std::string> typeMap;
        };

    }  // namespace Wake
}  // namespace UKControllerPlugin

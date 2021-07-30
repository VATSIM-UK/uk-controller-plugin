#pragma once

namespace UKControllerPlugin {
    namespace Wake {

        /*
            Class that maps aircraft types to their
            UK wake categories.
        */
        class WakeCategoryMapper
        {
            public:
                void AddCategoryMapping(std::string type, std::string category);
                int Count(void) const;

                std::string GetCategoryForAircraftType(
                    std::string aircraftType
                ) const;

                const std::string noCategory = "";
            private:
                // Maps ICAO code to wake category
                std::map<std::string, std::string> categoryMap;
        };

    }  // namespace Wake
}  // namespace UKControllerPlugin

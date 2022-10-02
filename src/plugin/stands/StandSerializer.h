#pragma once
#include "stands/Stand.h"
#include "stands/CompareStands.h"

namespace UKControllerPlugin {
    namespace Stands {

        /*
            Create Stand data from JSON
        */
        void from_json(
            const nlohmann::json& json,
            std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands>& stands);

        /*
            Returns true if the dependency data is valid
        */
        bool DependencyValid(const nlohmann::json& data);

        /*
            Returns true if the data for a specific airport is valid
        */
        bool AirportDataValid(const nlohmann::json& data);

        /*
            Returns true if individual stand data is valid
        */
        bool StandDataValid(const nlohmann::json& data);

    } // namespace Stands
} // namespace UKControllerPlugin

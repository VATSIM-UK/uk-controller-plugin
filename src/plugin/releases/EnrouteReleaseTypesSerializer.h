#pragma once
#include "releases/EnrouteReleaseType.h"
#include "releases/CompareEnrouteReleaseTypes.h"

namespace UKControllerPlugin {
    namespace Releases {

        /*
            Create HoldingData from JSON
        */
        void from_json(
            const nlohmann::json& json,
            std::set<
                UKControllerPlugin::Releases::EnrouteReleaseType,
                UKControllerPlugin::Releases::CompareEnrouteReleaseTypes>& releases);

        /*
            Returns true if the dependency structure is valid
        */
        bool DependencyValid(const nlohmann::json& data);

        /*
            Returns true if the holding data is valid
        */
        bool JsonValid(const nlohmann::json& data);
    } // namespace Releases
} // namespace UKControllerPlugin

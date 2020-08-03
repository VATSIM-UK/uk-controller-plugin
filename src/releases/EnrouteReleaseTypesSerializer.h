#pragma once
#include "releases/EnrouteReleaseType.h"

namespace UKControllerPlugin {
    namespace Releases {
        extern const EnrouteReleaseType releaseTypeInvalid;

        /*
            Create HoldingData from JSON
        */
        void from_json(
            const nlohmann::json& json,
            EnrouteReleaseType& releaseType
        );

        /*
            Returns true if the holding data is valid
        */
        bool JsonValid(const nlohmann::json& data);
    }  // namespace Releases
}  // namespace UKControllerPlugin

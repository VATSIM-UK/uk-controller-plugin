#pragma once
#include "releases/EnrouteReleaseType.h"

namespace UKControllerPlugin {
    namespace Releases {

        /*
            Used for comparing Enroute Release Type structs
        */
        typedef struct CompareEnrouteReleaseTypes
        {
            using is_transparent = int;

            bool operator()(
                const UKControllerPlugin::Releases::EnrouteReleaseType& releaseType,
                int id
            ) const;
            bool operator()(
                int id,
                const UKControllerPlugin::Releases::EnrouteReleaseType& releaseType
            ) const;
            bool operator()(
                const UKControllerPlugin::Releases::EnrouteReleaseType& releaseTypeA,
                const UKControllerPlugin::Releases::EnrouteReleaseType& releaseTypeB
            ) const;

        } CompareEnrouteReleaseTypes;
    }  // namespace Releases
}  // namespace UKControllerPlugin

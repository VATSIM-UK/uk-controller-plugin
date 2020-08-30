#include "pch/stdafx.h"
#include "releases/CompareEnrouteReleaseTypes.h"
#include "releases/EnrouteReleaseType.h"

namespace UKControllerPlugin {
    namespace Releases {

        bool CompareEnrouteReleaseTypes::operator()(const EnrouteReleaseType& releaseType, int id) const
        {
            return releaseType.id < id;
        }

        bool CompareEnrouteReleaseTypes::operator()(int id, const EnrouteReleaseType& releaseType) const
        {
            return id < releaseType.id;
        }

        bool CompareEnrouteReleaseTypes::operator()(
            const EnrouteReleaseType& releaseTypeA,
            const EnrouteReleaseType& releaseTypeB
        ) const {
            return releaseTypeA.id < releaseTypeB.id;
        }
    }  // namespace Releases
}  // namespace UKControllerPlugin

#pragma once
#include "intention/IntentionCodeData.h"

// Forward declare
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            A cache that maps aircraft callsign to intention code so we don't
            have to work it out every single tag call.
        */
        class IntentionCodeCache
        {
            public:
                bool IntentionCodeValid(
                    std::string callsign,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
                );
                std::string GetIntentionCodeForAircraft(std::string callsign) const;
                bool HasIntentionCodeForAircraft(std::string callsign) const;
                void RegisterAircraft(std::string callsign, UKControllerPlugin::IntentionCode::IntentionCodeData);
                size_t TotalCached(void) const;
                void UnregisterAircraft(std::string callsign);

                // The default intention code if we cant resolve something better
                const std::string defaultCode = "--";

                // The value given to us by Euroscope if we try to find the time to a point that has been passed.
                const int exitPointPassed = -1;


            private:
                std::map<std::string, UKControllerPlugin::IntentionCode::IntentionCodeData> intentionCodeMap;

        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

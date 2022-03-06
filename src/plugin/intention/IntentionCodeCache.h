#pragma once
#include "intention/IntentionCodeData.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::IntentionCode {

    /*
        A cache that maps aircraft callsign to intention code so we don't
        have to work it out every single tag call.
    */
    class IntentionCodeCache
    {
        public:
        void Clear(void);
        std::string GetIntentionCodeForAircraft(const std::string& callsign) const;
        bool HasIntentionCodeForAircraft(const std::string& callsign) const;
        auto GetDataForAircraft(const std::string& callsign) const -> IntentionCodeData;
        void RegisterAircraft(const std::string& callsign, IntentionCodeData);
        size_t TotalCached(void) const;
        void UnregisterAircraft(const std::string& callsign);

        // The default intention code if we cant resolve something better
        const std::string defaultCode = "--";

        // The value given to us by Euroscope if we try to find the time to a point that has been passed.
        const int exitPointPassed = -1;

        private:
        std::map<std::string, IntentionCodeData> intentionCodeMap;
    };
} // namespace UKControllerPlugin::IntentionCode

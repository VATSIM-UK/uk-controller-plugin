#include "pch/stdafx.h"
#include "intention/AirfieldGroup.h"
#include "intention/HomeAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        HomeAirfieldGroup::HomeAirfieldGroup(void)
        {
            this->Initialise();
        }

        /*
            We only care about airfields that are located in the UK - have EG codes.
        */
        bool HomeAirfieldGroup::HasAirfield(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return airfield.compare(0, 2, "EG") == 0;
        }

        /*
            Return the last two letters of the destination ICAO.
        */
        std::string HomeAirfieldGroup::GetIntentionCodeForGroup(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return airfield.substr(airfield.length() - 2);
        }

        /*
            As this is the UK, we don't actually need any airfields here, for now.
        */
        bool HomeAirfieldGroup::Initialise(void)
        {
            return true;
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

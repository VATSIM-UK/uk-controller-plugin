#include "pch/stdafx.h"
#include "intention/AirfieldGroup.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        bool AirfieldGroup::AppliesToController(
            std::string callsign,
            UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route
        ) const
        {
            return true;
        }

        /*
            Adds a particular airfield to the list of airfields.
        */
        void AirfieldGroup::AddAirfieldToList(std::string airfield)
        {
            this->airfields.push_back(airfield);
        }

        /*
            Returns true if the airfield ICAO string matches one in the list.
        */
        bool AirfieldGroup::AirfieldInList(std::string airfield) const
        {
            return std::find(this->airfields.begin(), this->airfields.end(), airfield) != this->airfields.end();
        }

        bool AirfieldGroup::ControllerIsScottish(std::string callsign) const
        {
            std::string prefix = callsign.substr(0, 3);
            return prefix == this->SCAC_CALLSIGN_PREFIX ||
                prefix == this->SCTC_CALLSIGN_PREFIX ||
                prefix == this->EGPX_CALLSIGN_PREFIX ||
                prefix == this->EGPX_NI_CALLSIGN_PREFIX ||
                prefix == this->EGPX_MIL_CALLSIGN_PREFIX;
        }

        /*
            By default, we empty the airfield list.
        */
        bool AirfieldGroup::Initialise(void)
        {
            return this->airfields.empty();
        }

        /*
            Returns true if a particular point is found in the flightplan, which may be used change the intention code
            or whether or not the airfield is "known".
        */
        bool AirfieldGroup::IsViaPoint(std::string point, EuroscopeExtractedRouteInterface & route) const
        {
            int i = 0;
            while (i < route.GetPointsNumber()) {
                if (point.compare(route.GetPointName(i)) == 0) {
                    return true;
                }

                i++;
            }

            return false;
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

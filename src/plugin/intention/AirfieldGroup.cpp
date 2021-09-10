#include "AirfieldGroup.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {

    AirfieldGroup::AirfieldGroup() = default;
    AirfieldGroup::~AirfieldGroup() = default;
    AirfieldGroup::AirfieldGroup(const AirfieldGroup&) = default;
    AirfieldGroup::AirfieldGroup(AirfieldGroup&&) = default;

    auto AirfieldGroup::AppliesToController(
        const std::string& callsign, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
        -> bool
    {
        return true;
    }

    /*
        Adds a particular airfield to the list of airfields.
    */
    void AirfieldGroup::AddAirfieldToList(const std::string& airfield)
    {
        this->airfields.push_back(airfield);
    }

    /*
        Returns true if the airfield ICAO string matches one in the list.
    */
    auto AirfieldGroup::AirfieldInList(const std::string& airfield) const -> bool
    {
        return std::find(this->airfields.begin(), this->airfields.end(), airfield) != this->airfields.end();
    }

    auto AirfieldGroup::ControllerIsScottish(const std::string& callsign) const -> bool
    {
        std::string prefix = callsign.substr(0, 3);
        return prefix == this->SCAC_CALLSIGN_PREFIX || prefix == this->SCTC_CALLSIGN_PREFIX ||
               prefix == this->EGPX_CALLSIGN_PREFIX || prefix == this->EGPX_NI_CALLSIGN_PREFIX ||
               prefix == this->EGPX_MIL_CALLSIGN_PREFIX;
    }

    /*
        By default, we empty the airfield list.
    */
    auto AirfieldGroup::Initialise() -> bool
    {
        return this->airfields.empty();
    }

    /*
        Returns true if a particular point is found in the flightplan, which may be used change the intention code
        or whether or not the airfield is "known".
    */
    auto AirfieldGroup::IsViaPoint(const std::string& point, EuroscopeExtractedRouteInterface& route) -> bool
    {
        int i = 0;
        while (i < route.GetPointsNumber()) {
            if (point == route.GetPointName(i)) {
                return true;
            }

            i++;
        }

        return false;
    };
} // namespace UKControllerPlugin::IntentionCode

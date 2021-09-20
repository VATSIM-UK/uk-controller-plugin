#include "HomeAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {

    HomeAirfieldGroup::HomeAirfieldGroup()
    {
        this->Initialise(); // NOLINT
    }

    /*
        We only care about airfields that are located in the UK - have EG codes.
    */
    auto HomeAirfieldGroup::HasAirfield(const std::string& airfield, EuroscopeExtractedRouteInterface& route) const
        -> bool
    {
        return airfield.compare(0, 2, "EG") == 0;
    }

    /*
        Return the last two letters of the destination ICAO.
    */
    auto HomeAirfieldGroup::GetIntentionCodeForGroup(
        const std::string& airfield, EuroscopeExtractedRouteInterface& route) const -> std::string
    {
        return airfield.substr(airfield.length() - 2);
    }

    /*
        As this is the UK, we don't actually need any airfields here, for now.
    */
    auto HomeAirfieldGroup::Initialise() -> bool
    {
        return true;
    }
} // namespace UKControllerPlugin::IntentionCode

#include "DublinAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {
    DublinAirfieldGroup::DublinAirfieldGroup()
    {
        this->Initialise(); // NOLINT
    }

    /*
        We're only interested in a small number of airfields beginning with "EI".
    */
    auto DublinAirfieldGroup::HasAirfield(const std::string& airfield, EuroscopeExtractedRouteInterface& route) const
        -> bool
    {
        return airfield.compare(0, 2, "EI") == 0 && this->AirfieldInList(airfield);
    }

    /*
        Dublin TMA Landers code.
    */
    auto DublinAirfieldGroup::GetIntentionCodeForGroup(
        const std::string& airfield, EuroscopeExtractedRouteInterface& route) const -> std::string
    {
        return "DW";
    }

    /*
        Adds airfields to the list.
    */
    auto DublinAirfieldGroup::Initialise() -> bool
    {
        AirfieldGroup::Initialise();
        this->AddAirfieldToList("EIDW");
        this->AddAirfieldToList("EIME");
        this->AddAirfieldToList("EIWT");
        this->AddAirfieldToList("EINC");
        this->AddAirfieldToList("EITM");
        this->AddAirfieldToList("EITT");

        return true;
    }
} // namespace UKControllerPlugin::IntentionCode

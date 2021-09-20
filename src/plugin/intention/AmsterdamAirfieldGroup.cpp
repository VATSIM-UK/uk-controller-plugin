#include "AmsterdamAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {
    AmsterdamAirfieldGroup::AmsterdamAirfieldGroup()
    {
        this->Initialise(); // NOLINT
    }

    auto AmsterdamAirfieldGroup::AppliesToController(
        const std::string& callsign, EuroscopeExtractedRouteInterface& route) const -> bool
    {
        return !this->ControllerIsScottish(callsign) || IsViaPoint("TOPPA", route);
    }

    /*
        We only care about airfields beginning with EH (Amsterdam).
    */
    auto AmsterdamAirfieldGroup::HasAirfield(const std::string& airfield, EuroscopeExtractedRouteInterface& route) const
        -> bool
    {
        return airfield.compare(0, 2, "EH") == 0 && this->AirfieldInList(airfield);
    }

    /*
        If the route is via KOK, we use the "AS" code, if not, it's the standard "AM".
    */
    auto AmsterdamAirfieldGroup::GetIntentionCodeForGroup(
        const std::string& airfield, EuroscopeExtractedRouteInterface& route) const -> std::string
    {
        return IsViaPoint("KOK", route) ? this->CODE_VIA_KOKSY : this->CODE_NOT_VIA_KOKSY;
    }

    /*
        Create the list of airfields that we are concerned about.
    */
    auto AmsterdamAirfieldGroup::Initialise() -> bool
    {
        AirfieldGroup::Initialise();
        this->AddAirfieldToList("EHAM");
        this->AddAirfieldToList("EHEH");
        this->AddAirfieldToList("EHLE");
        this->AddAirfieldToList("EHSB");
        this->AddAirfieldToList("EHYB");
        this->AddAirfieldToList("EHVK");
        this->AddAirfieldToList("EHMZ");
        this->AddAirfieldToList("EHBD");
        this->AddAirfieldToList("EHBK");
        this->AddAirfieldToList("EHKD");
        this->AddAirfieldToList("EHRD");
        this->AddAirfieldToList("EHVB");
        this->AddAirfieldToList("EHWO");
        this->AddAirfieldToList("EHGR");
        this->AddAirfieldToList("EHSE");
        this->AddAirfieldToList("EHDP");

        return true;
    }
} // namespace UKControllerPlugin::IntentionCode

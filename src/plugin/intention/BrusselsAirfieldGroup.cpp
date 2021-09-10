#include "BrusselsAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {
    BrusselsAirfieldGroup::BrusselsAirfieldGroup()
    {
        this->Initialise(); // NOLINT
    }

    auto BrusselsAirfieldGroup::AppliesToController(
        const std::string& callsign, EuroscopeExtractedRouteInterface& route) const -> bool
    {
        return !this->ControllerIsScottish(callsign);
    }

    /*
        We only recognise the airfield if it's in Brussels AND the aircraft is going via KOK.
        Other cases use the intention code determined by the sector exit point in IntentionCode.
    */
    auto BrusselsAirfieldGroup::HasAirfield(const std::string& airfield, EuroscopeExtractedRouteInterface& route) const
        -> bool
    {
        return airfield.compare(0, 2, "EB") == 0 &&
               (this->primaryAirfields.count(airfield) != 0 || this->secondaryAirfields.count(airfield) != 0) &&
               IsViaPoint("KOK", route);
    }

    /*
        Only one intention code here.
    */
    auto BrusselsAirfieldGroup::GetIntentionCodeForGroup(
        const std::string& airfield, EuroscopeExtractedRouteInterface& route) const -> std::string
    {
        return this->primaryAirfields.count(airfield) != 0 ? this->PRIMARY_CODE : this->SECONDARY_CODE;
    }

    /*
        Add the airports to the list.
    */
    auto BrusselsAirfieldGroup::Initialise() -> bool
    {
        AirfieldGroup::Initialise();
        return true;
    }
} // namespace UKControllerPlugin::IntentionCode

#include "pch/pch.h"
#include "intention/AirfieldGroup.h"
#include "intention/BrusselsAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {
        BrusselsAirfieldGroup::BrusselsAirfieldGroup(void)
        {
            this->Initialise();
        }

        bool BrusselsAirfieldGroup::AppliesToController(
            std::string callsign,
            EuroscopeExtractedRouteInterface& route
        ) const {
            return !this->ControllerIsScottish(callsign);
        }

        /*
            We only recognise the airfield if it's in Brussels AND the aircraft is going via KOK.
            Other cases use the intention code determined by the sector exit point in IntentionCode.
        */
        bool BrusselsAirfieldGroup::HasAirfield(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return airfield.compare(0, 2, "EB") == 0 &&
                (this->primaryAirfields.count(airfield) || this->secondaryAirfields.count(airfield)) &&
                this->IsViaPoint("KOK", route);
        }

        /*
            Only one intention code here.
        */
        std::string BrusselsAirfieldGroup::GetIntentionCodeForGroup(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return this->primaryAirfields.count(airfield)
                ? this->PRIMARY_CODE
                : this->SECONDARY_CODE;
        }

        /*
            Add the airports to the list.
        */
        bool BrusselsAirfieldGroup::Initialise(void)
        {
            AirfieldGroup::Initialise();
            return true;
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

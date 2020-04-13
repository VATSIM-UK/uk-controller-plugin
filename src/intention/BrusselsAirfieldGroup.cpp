#include "pch/stdafx.h"
#include "intention/AirfieldGroup.h"
#include "intention/BrusselsAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {
        BrusselsAirfieldGroup::BrusselsAirfieldGroup(void)
        {
            this->Initialise();
        }

        bool BrusselsAirfieldGroup::AppliesToController(std::string callsign, EuroscopeExtractedRouteInterface& route) const
        {
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
                this->AirfieldInList(airfield) && this->IsViaPoint("KOK", route);
        }

        /*
            Only one intention code here.
        */
        std::string BrusselsAirfieldGroup::GetIntentionCodeForGroup(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return "EB";
        }

        /*
            Add the airports to the list.
        */
        bool BrusselsAirfieldGroup::Initialise(void)
        {
            AirfieldGroup::Initialise();
            this->AddAirfieldToList("EBBR");
            this->AddAirfieldToList("EBCI");
            this->AddAirfieldToList("EBAW");
            this->AddAirfieldToList("EBKT");
            this->AddAirfieldToList("EBMB");
            this->AddAirfieldToList("EBCV");
            this->AddAirfieldToList("EBLG");

            return true;
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

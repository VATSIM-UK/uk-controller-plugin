#include "pch/stdafx.h"
#include "intention/AirfieldGroup.h"
#include "intention/AmsterdamAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {
        AmsterdamAirfieldGroup::AmsterdamAirfieldGroup(void)
        {
            this->Initialise();
        }

        /*
            We only care about airfields beginning with EH (Amsterdam).
        */
        bool AmsterdamAirfieldGroup::HasAirfield(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return airfield.compare(0, 2, "EH") == 0 && this->AirfieldInList(airfield);
        }

        /*
            If the route is via KOK, we use the "AS" code, if not, it's the standard "AM".
        */
        std::string AmsterdamAirfieldGroup::GetIntentionCodeForGroup(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return (this->IsViaPoint("KOK", route)) ? "AS" : "AM";
        }

        /*
            Create the list of airfields that we are concerned about.
        */
        bool AmsterdamAirfieldGroup::Initialise(void)
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
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

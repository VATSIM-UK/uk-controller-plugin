#include "pch/pch.h"
#include "intention/AirfieldGroup.h"
#include "intention/DublinAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {
        DublinAirfieldGroup::DublinAirfieldGroup(void)
        {
            this->Initialise();
        }

        /*
            We're only interested in a small number of airfields beginning with "EI".
        */
        bool DublinAirfieldGroup::HasAirfield(std::string airfield, EuroscopeExtractedRouteInterface & route) const
        {
            return airfield.compare(0, 2, "EI") == 0 && this->AirfieldInList(airfield);
        }

        /*
            Dublin TMA Landers code.
        */
        std::string DublinAirfieldGroup::GetIntentionCodeForGroup(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return "DW";
        }

        /*
            Adds airfields to the list.
        */
        bool DublinAirfieldGroup::Initialise(void)
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
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

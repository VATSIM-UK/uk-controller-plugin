#include "pch/pch.h"
#include "intention/AirfieldGroup.h"
#include "intention/ShannonAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        ShannonAirfieldGroup::ShannonAirfieldGroup(void)
        {
            this->Initialise();
        }

        /*
            We only care about one airfield for now (Shannon).
        */
        bool ShannonAirfieldGroup::HasAirfield(std::string airfield, EuroscopeExtractedRouteInterface & route) const
        {
            return this->AirfieldInList(airfield);
        }

        /*
            We return the Shannon code.
        */
        std::string ShannonAirfieldGroup::GetIntentionCodeForGroup(
            std::string airfield,
            EuroscopeExtractedRouteInterface & route
        ) const {
            return "NN";
        }

        /*
            Create the list of airfields that we are concerned about.
        */
        bool ShannonAirfieldGroup::Initialise(void)
        {
            AirfieldGroup::Initialise();
            this->AddAirfieldToList("EINN");
            return true;
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

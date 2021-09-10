#include "ShannonAirfieldGroup.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {

    ShannonAirfieldGroup::ShannonAirfieldGroup()
    {
        this->Initialise(); // NOLINT
    }

    /*
        We only care about one airfield for now (Shannon).
    */
    auto ShannonAirfieldGroup::HasAirfield(const std::string& airfield, EuroscopeExtractedRouteInterface& route) const
        -> bool
    {
        return this->AirfieldInList(airfield);
    }

    /*
        We return the Shannon code.
    */
    auto ShannonAirfieldGroup::GetIntentionCodeForGroup(
        const std::string& airfield, EuroscopeExtractedRouteInterface& route) const -> std::string
    {
        return "NN";
    }

    /*
        Create the list of airfields that we are concerned about.
    */
    auto ShannonAirfieldGroup::Initialise() -> bool
    {
        AirfieldGroup::Initialise();
        this->AddAirfieldToList("EINN");
        return true;
    }
} // namespace UKControllerPlugin::IntentionCode

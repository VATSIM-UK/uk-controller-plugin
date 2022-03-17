#include "AirfieldApproachOptions.h"
#include "ApproachSequencerOptions.h"

namespace UKControllerPlugin::Approach {
    void ApproachSequencerOptions::Set(const std::string& airfield, std::shared_ptr<AirfieldApproachOptions> options)
    {
        airfieldOptions[airfield] = options;
    }

    auto ApproachSequencerOptions::Get(const std::string& airfield) -> const AirfieldApproachOptions&
    {
        if (airfieldOptions.count(airfield) == 0) {
            airfieldOptions[airfield] = std::make_shared<AirfieldApproachOptions>();
        }

        return *airfieldOptions.at(airfield);
    }
} // namespace UKControllerPlugin::Approach

#include "ApproachSequencerDisplayOptions.h"

namespace UKControllerPlugin::Approach {

    auto ApproachSequencerDisplayOptions::Airfield() const -> const std::string&
    {
        return airfield;
    }

    void ApproachSequencerDisplayOptions::Airfield(const std::string& airfield)
    {
        this->airfield = airfield;
    }
} // namespace UKControllerPlugin::Approach

#include "ApproachSequencerOptions.h"

namespace UKControllerPlugin::Approach {

    void ApproachSequencerOptions::DefaultMode(const std::string& airfield, ApproachSequencingMode mode)
    {
        defaultModes[airfield] = mode;
    }

    auto ApproachSequencerOptions::DefaultMode(const std::string& airfield) const -> ApproachSequencingMode
    {
        auto mode = defaultModes.find(airfield);
        return mode == defaultModes.cend() ? DEFAULT_MODE : mode->second;
    }

    void ApproachSequencerOptions::MinimumSeparation(const std::string& airfield, double separation)
    {
        minimumSeparations[airfield] = separation;
    }

    auto ApproachSequencerOptions::MinimumSeparation(const std::string& airfield) const -> double
    {
        auto separation = minimumSeparations.find(airfield);
        return separation == minimumSeparations.cend() ? DEFAULT_SEPARATION : separation->second;
    }

    void ApproachSequencerOptions::TargetDistance(const std::string& airfield, double target)
    {
        targetDistances[airfield] = target;
    }

    auto ApproachSequencerOptions::TargetDistance(const std::string& airfield) const -> double
    {
        auto target = targetDistances.find(airfield);
        return target == targetDistances.cend() ? DEFAULT_TARGET_DISTANCE : target->second;
    }
} // namespace UKControllerPlugin::Approach

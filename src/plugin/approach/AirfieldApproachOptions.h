#pragma once
#include "ApproachSequencingMode.h"

namespace UKControllerPlugin::Approach {
    using AirfieldApproachOptions = struct AirfieldApproachOptions
    {
        AirfieldApproachOptions() = default;

        AirfieldApproachOptions(
            ApproachSequencingMode defaultMode, double targetDistance, double minimumSeparationRequirement)
            : defaultMode(defaultMode), targetDistance(targetDistance),
              minimumSeparationRequirement(minimumSeparationRequirement)
        {
        }

        // The default mode to use
        ApproachSequencingMode defaultMode = ApproachSequencingMode::WakeTurbulence;

        // The default target distance to use
        double targetDistance = 6.0;

        // The minimum separation requirement
        double minimumSeparationRequirement = 3.0;
    };
} // namespace UKControllerPlugin::Approach

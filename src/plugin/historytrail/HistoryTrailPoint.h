#pragma once

namespace UKControllerPlugin::HistoryTrail {
    using HistoryTrailPoint = struct HistoryTrailPoint
    {
        HistoryTrailPoint(double heading, EuroScopePlugIn::CPosition position)
            : heading(std::move(heading)), position(std::move(position))
        {
        }

        // Heading
        double heading;

        // Position
        EuroScopePlugIn::CPosition position;
    };
} // namespace UKControllerPlugin::HistoryTrail

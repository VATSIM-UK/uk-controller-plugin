#pragma once

namespace UKControllerPlugin::MissedApproach {
    class MissedApproach;

    /**
     * Compares prenotes in a set by id.
     */
    using CompareMissedApproaches = struct CompareMissedApproaches
    {
        using is_transparent = std::string;

        auto operator()(const std::shared_ptr<MissedApproach>& missed, const std::string& callsign) const -> bool;
        auto operator()(const std::string& callsign, const std::shared_ptr<MissedApproach>& missed) const -> bool;
        auto operator()(const std::shared_ptr<MissedApproach>& a, const std::shared_ptr<MissedApproach>& b) const
            -> bool;
    };
} // namespace UKControllerPlugin::MissedApproach

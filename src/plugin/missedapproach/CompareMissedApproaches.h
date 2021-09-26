#pragma once

namespace UKControllerPlugin::MissedApproach {
    class MissedApproach;

    /**
     * Compares prenotes in a set by id.
     */
    using CompareMissedApproaches = struct CompareMissedApproaches
    {
        using is_transparent = int;

        auto operator()(const std::shared_ptr<MissedApproach>& missed, int id) const -> bool;
        auto operator()(int id, const std::shared_ptr<MissedApproach>& missed) const -> bool;
        auto operator()(const std::shared_ptr<MissedApproach>& a, const std::shared_ptr<MissedApproach>& b) const
            -> bool;
    };
} // namespace UKControllerPlugin::MissedApproach

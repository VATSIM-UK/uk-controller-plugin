#pragma once

namespace UKControllerPlugin::Hold {
    struct ProximityHold;

    using CompareProximityHolds = struct CompareProximityHolds
    {
        using is_transparent = std::string;

        bool operator()(const std::shared_ptr<ProximityHold>& proximity, std::string navaid) const;
        bool operator()(std::string navaid, const std::shared_ptr<ProximityHold>& proximity) const;
        bool operator()(const std::shared_ptr<ProximityHold>& a, const std::shared_ptr<ProximityHold>& b) const;
    };
} // namespace UKControllerPlugin::Hold

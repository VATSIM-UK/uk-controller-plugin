#pragma once

namespace UKControllerPlugin::Releases {

    using EnrouteRelease = struct EnrouteRelease
    {
        // The type of release
        int releaseType;

        // The release point
        std::string releasePoint;

        // The time at which this item should be cleared
        std::chrono::system_clock::time_point clearTime;

        [[nodiscard]] auto operator==(const EnrouteRelease& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Releases

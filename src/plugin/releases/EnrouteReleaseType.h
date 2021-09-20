#pragma once

namespace UKControllerPlugin::Releases {

    using EnrouteReleaseType = struct EnrouteReleaseType
    {
        int id;

        // What to show in the tag
        std::string tagString;

        // The description for menus
        std::string description;

        [[nodiscard]] auto operator==(const EnrouteReleaseType& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Releases

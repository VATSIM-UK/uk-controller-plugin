#pragma once

namespace UKControllerPlugin::Squawk {
    /*
        Represents the allocation of a squawk by the Web API
    */
    using ApiSquawkAllocation = struct ApiSquawkAllocation
    {
        // The callsign the squawk was assigned to
        const std::string callsign;

        // The code that was assigned
        const std::string squawk;

        [[nodiscard]] auto operator==(const ApiSquawkAllocation& compare) const -> bool;
        [[nodiscard]] auto operator<(const ApiSquawkAllocation& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Squawk

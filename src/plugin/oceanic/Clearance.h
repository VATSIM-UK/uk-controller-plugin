#pragma once

namespace UKControllerPlugin::Oceanic {

    /**
     * Represents an oceanic clearance.
     *
     * We have clearance, Clarance.
     */
    using Clearance = struct Clearance
    {
        // Callsign on the clearance
        std::string callsign;

        // The current status
        std::string status;

        // The track (or RR) that they're cleared on.
        std::string track;

        // Entry fix
        std::string entryFix;

        // Flight level
        std::string flightLevel;

        // Mach number
        std::string mach;

        // Entry estimate time (Z)
        std::string entryTime;

        // Time (Z) the clearance was issued
        std::string clearanceIssued;

        // Extra information
        std::string extra;

        ~Clearance();
        Clearance(const Clearance& compare);
        Clearance(Clearance&& compare) = delete;
        [[nodiscard]] auto operator=(const Clearance& compare) -> Clearance&;
        [[nodiscard]] auto operator=(Clearance&& compare) -> Clearance& = delete;
        auto operator==(const Clearance& compare) const -> bool;
        void AssignProperties(const Clearance& compare);

        const std::string CLEARANCE_STATUS_CLEARED = "CLEARED";
        const std::string CLEARANCE_STATUS_PENDING = "PENDING";
    };
} // namespace UKControllerPlugin::Oceanic

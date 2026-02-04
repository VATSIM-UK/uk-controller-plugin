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

        explicit Clearance(std::string callsign);
        Clearance(
            std::string callsign,
            std::string status,
            std::string track,
            std::string entryFix,
            std::string flightLevel,
            std::string mach,
            std::string entryTime,
            std::string clearanceIssued,
            std::string extra);
        ~Clearance();
        Clearance(const Clearance& compare);
        Clearance(Clearance&& compare) noexcept;
        auto operator=(const Clearance& compare) -> Clearance&;
        auto operator=(Clearance&& compare) noexcept -> Clearance&;
        auto operator==(const Clearance& compare) const -> bool;
        void AssignProperties(const Clearance& compare);
        void MoveProperties(Clearance& compare);

        const std::string CLEARANCE_STATUS_CLEARED = "CLEARED";
        const std::string CLEARANCE_STATUS_PENDING = "PENDING";
    };
} // namespace UKControllerPlugin::Oceanic
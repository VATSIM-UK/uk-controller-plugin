#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace Oceanic {

        /**
         * Represents an oceanic clearance.
         *
         * We have clearance, Clarance.
         */
        typedef struct Clearance
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

            bool operator==(const Clearance& compare) const
            {
                return this->callsign == compare.callsign;
            }

            void operator=(const Clearance& compare)
            {
                this->callsign = compare.callsign;
                this->status = compare.status;
                this->track = compare.track;
                this->entryFix = compare.entryFix;
                this->flightLevel = compare.flightLevel;
                this->mach = compare.mach;
                this->entryTime = compare.entryTime;
                this->clearanceIssued = compare.clearanceIssued;
                this->extra = compare.extra;
            }

            const std::string CLEARANCE_STATUS_CLEARED = "CLEARED";
            const std::string CLEARANCE_STATUS_PENDING = "PENDING";
        } Clearance;
    } // namespace Oceanic
} // namespace UKControllerPlugin

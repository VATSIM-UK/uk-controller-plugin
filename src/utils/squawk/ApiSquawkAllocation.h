#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace Squawk {
        /*
            Represents the allocation of a squawk by the Web API
        */
        typedef struct ApiSquawkAllocation {
            // The callsign the squawk was assigned to
            const std::string callsign;

            // The code that was assigned
            const std::string squawk;

            bool operator== (const ApiSquawkAllocation & compare) const
            {
                return this->callsign == compare.callsign
                    && this->squawk == compare.squawk;
            }

            bool operator< (const ApiSquawkAllocation & compare) const
            {
                return this->callsign < compare.callsign;
            }
        } ApiSquawkAllocation;
    }  // namespace Squawk
}  // namespace UKControllerPlugin

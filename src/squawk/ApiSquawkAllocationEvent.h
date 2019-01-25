#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Squawk {
        /*
            Represents the allocation of a squawk by the Web API
        */
        typedef struct ApiSquawkAllocationEvent {
            // The callsign the squawk was assigned to
            const std::string callsign;

            // The code that was assigned
            const std::string squawk;

            bool operator== (const ApiSquawkAllocationEvent & compare) const
            {
                return this->callsign == compare.callsign
                    && this->squawk == compare.squawk;
            }

            bool operator< (const ApiSquawkAllocationEvent & compare) const
            {
                return this->callsign < compare.callsign;
            }
        } ApiSquawkAllocationEvent;
    }  // namespace Squawk
}  // namespace UKControllerPlugin

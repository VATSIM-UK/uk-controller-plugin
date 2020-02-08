#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Handoff {

        /*
            Represents a handoff that has been cached.
        */
        struct CachedHandoff
        {
            public:

                CachedHandoff()
                    : frequency(""), callsign("")
                {

                };


                CachedHandoff(std::string frequency, std::string callsign)
                    : frequency(frequency), callsign(callsign)
                {

                };

                // The frequency that the handoff is to
                std::string frequency;

                // The normalised callsign of the position that the handoff is to
                std::string callsign;

                bool operator==(const CachedHandoff& compare) const
                {
                    return this->callsign == compare.callsign &&
                        this->frequency == compare.frequency;
                }
        };
    }  // namespace UKControllerPlugin
}  // namespacem Handoff

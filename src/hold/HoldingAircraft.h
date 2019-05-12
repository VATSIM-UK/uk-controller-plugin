#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Data about a holding aircraft
        */
        typedef struct HoldingAircraft
        {
            const std::string callsign;
            int clearedLevel;
            int reportedLevel;
            int verticalSpeed;
            const std::chrono::system_clock::time_point entryTime;

            void operator=(const HoldingAircraft & newItem)
            {
                this->clearedLevel = newItem.clearedLevel;
                this->reportedLevel = newItem.reportedLevel;
                this->verticalSpeed = newItem.verticalSpeed;
            }

            bool operator== (const HoldingAircraft & compare) const {
                return this->callsign == compare.callsign;
            }

            bool operator== (const std::string & compare) const {
                return this->callsign == compare;
            }
        } HoldingAircraft;

    }  // namespace Hold
}  // namespace UKControllerPlugin

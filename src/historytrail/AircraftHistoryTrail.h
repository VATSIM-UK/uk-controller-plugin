#pragma once
#include "historytrail/HistoryTrailPoint.h"

namespace UKControllerPlugin {
    namespace HistoryTrail {
        /*
            Class that encapsulates a standard C++ double-ended queue with a maximum size
            to provide a list of aircraft positions.
        */
        class AircraftHistoryTrail
        {
        public:
            explicit AircraftHistoryTrail(std::string callsign);
            void AddItem(HistoryTrailPoint point);
            std::string GetCallsign(void) const;
            const std::deque<HistoryTrailPoint>& GetTrail(void) const;

            // The maximum number of items we can have in the history trail.
            const unsigned int maxSize = 50;
        private:
            // A queue of aircraft positions.
            std::deque<HistoryTrailPoint> trail;

            // Aircraft callsign corresponding to the history trail
            std::string callsign;
        };
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin

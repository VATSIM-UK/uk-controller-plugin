#pragma once
#include "HistoryTrailPoint.h"

namespace UKControllerPlugin::HistoryTrail {
    /*
        Class that encapsulates a standard C++ double-ended queue with a maximum size
        to provide a list of aircraft positions.
    */
    class AircraftHistoryTrail
    {
        public:
        explicit AircraftHistoryTrail(std::string callsign);
        void AddItem(const HistoryTrailPoint& point);
        [[nodiscard]] auto GetCallsign() const -> std::string;
        [[nodiscard]] auto GetTrail(void) const -> const std::vector<HistoryTrailPoint>&;

        // The maximum number of items we can have in the history trail.
        const unsigned int maxSize = 50;

        private:
        // A queue of aircraft positions.
        std::vector<HistoryTrailPoint> trail;

        // Aircraft callsign corresponding to the history trail
        std::string callsign;
    };
} // namespace UKControllerPlugin::HistoryTrail

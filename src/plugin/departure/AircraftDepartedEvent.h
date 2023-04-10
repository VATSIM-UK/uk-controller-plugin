#pragma once

namespace UKControllerPlugin::Departure {
    /**
     * Event fired when an aircraft departs.
     */
    struct AircraftDepartedEvent
    {
        // The aircraft
        std::string callsign;

        // The airfield
        std::string airfield;
    };
} // namespace UKControllerPlugin::Departure

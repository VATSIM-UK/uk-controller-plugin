#pragma once

namespace UKControllerPlugin::Departure {
    /**
     * An event fired when a user should be clearing the departure data (e.g. initial altitude,
     * initial heading) of an aircraft.
     */
    using UserShouldClearDepartureDataEvent = struct UserShouldClearDepartureDataEvent
    {
        // Callsign of the aircraft
        std::string callsign;
    };
} // namespace UKControllerPlugin::Departure

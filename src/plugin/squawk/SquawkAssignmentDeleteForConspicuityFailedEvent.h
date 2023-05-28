#pragma once

namespace UKControllerPlugin::Squawk {
    using SquawkAssignmentDeleteForConspicuityFailedEvent = struct SquawkAssignmentDeleteForConspicuityFailedEvent
    {
        std::string callsign;

        std::string previousSquawk;
    };
} // namespace UKControllerPlugin::Squawk

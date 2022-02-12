#pragma once

namespace UKControllerPlugin::Hold {
    using ProximityHold = struct ProximityHold
    {
        ProximityHold(std::string callsign, std::string navaid, std::chrono::system_clock::time_point enteredAt)
            : callsign(std::move(callsign)), navaid(std::move(navaid)), enteredAt(std::move(enteredAt))
        {
        }

        std::string callsign;

        std::string navaid;

        std::chrono::system_clock::time_point enteredAt;
    };
} // namespace UKControllerPlugin::Hold

#pragma once

namespace UKControllerPlugin::Controller {

    class ActiveCallsign;

    using CompareActiveCallsigns = struct CompareActiveCallsigns
    {
        using is_transparent = std::string;

        bool operator()(const std::shared_ptr<ActiveCallsign>& callsign, std::string callsignString) const;
        bool operator()(std::string callsignString, const std::shared_ptr<ActiveCallsign>& callsign) const;
        bool operator()(const std::shared_ptr<ActiveCallsign>& a, const std::shared_ptr<ActiveCallsign>& b) const;
    };
} // namespace UKControllerPlugin::Controller

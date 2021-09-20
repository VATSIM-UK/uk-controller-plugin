#pragma once

namespace UKControllerPlugin::Handoff {

    /*
        Represents a handoff that has been cached.
    */
    struct CachedHandoff
    {
        public:
        CachedHandoff();
        CachedHandoff(std::string frequency, std::string callsign);
        [[nodiscard]] auto operator==(const CachedHandoff& compare) const -> bool;

        // The frequency that the handoff is to
        std::string frequency;

        // The normalised callsign of the position that the handoff is to
        std::string callsign;
    };
} // namespace UKControllerPlugin::Handoff

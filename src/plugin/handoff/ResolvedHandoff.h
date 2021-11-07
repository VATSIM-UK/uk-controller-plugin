#pragma once

namespace UKControllerPlugin::Handoff {

    /*
        Represents a handoff that has been cached.
    */
    using ResolvedHandoff = struct ResolvedHandoff
    {
        public:
        ResolvedHandoff(std::string frequency, std::string callsign);
        [[nodiscard]] auto operator==(const ResolvedHandoff& compare) const -> bool;

        // The frequency that the handoff is to
        const std::string frequency;

        // The normalised callsign of the position that the handoff is to
        const std::string callsign;
    };
} // namespace UKControllerPlugin::Handoff

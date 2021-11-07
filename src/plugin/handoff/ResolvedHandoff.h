#pragma once

namespace UKControllerPlugin::Handoff {

    /*
        Represents a handoff that has been cached.
    */
    using ResolvedHandoff = struct ResolvedHandoff
    {
        public:
        ResolvedHandoff(std::string callsign, double frequency);

        // The normalised callsign of the position that the handoff is to
        const std::string callsign;

        // The frequency that the handoff is to
        const double frequency;
    };
} // namespace UKControllerPlugin::Handoff

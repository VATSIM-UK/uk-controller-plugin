#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPositionHierarchy;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Handoff {

    /*
        Represents a handoff that has been cached.
    */
    using ResolvedHandoff = struct ResolvedHandoff
    {
        public:
        ResolvedHandoff(
            std::string callsign,
            double frequency,
            const std::shared_ptr<Controller::ControllerPositionHierarchy>& hierarchy);

        // The normalised callsign of the position that the handoff is to
        const std::string callsign;

        // The frequency that the handoff is to
        const double frequency;

        // The hierarchy of controllers this handoff is resolved too
        const std::shared_ptr<Controller::ControllerPositionHierarchy> hierarchy;
    };
} // namespace UKControllerPlugin::Handoff

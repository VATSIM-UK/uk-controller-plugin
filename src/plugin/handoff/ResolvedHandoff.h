#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPosition;
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
            const std::shared_ptr<Controller::ControllerPosition> resolvedController,
            const std::shared_ptr<Controller::ControllerPositionHierarchy> sidHierarchy,
            const std::shared_ptr<Controller::ControllerPositionHierarchy> airfieldHierarchy);

        // The normalised callsign of the position that the handoff is to
        const std::string callsign;

        // Which controller position this has resolved to
        const std::shared_ptr<Controller::ControllerPosition> resolvedController;

        // The hierarchy of controllers that the SID-specific handoff resolves to
        const std::shared_ptr<Controller::ControllerPositionHierarchy> sidHierarchy;

        // The hierarchy of airfield controllers that this resolves to
        const std::shared_ptr<Controller::ControllerPositionHierarchy> airfieldHierarchy;
    };
} // namespace UKControllerPlugin::Handoff

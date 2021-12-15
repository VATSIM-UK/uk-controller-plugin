#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPositionHierarchy;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Handoff {
    using HandoffOrder = struct HandoffOrder
    {
        HandoffOrder(int id, std::shared_ptr<Controller::ControllerPositionHierarchy> order);

        // The id of the handoff order
        const int id;

        // The controller position hierarchy
        const std::shared_ptr<Controller::ControllerPositionHierarchy> order;
    };
} // namespace UKControllerPlugin::Handoff

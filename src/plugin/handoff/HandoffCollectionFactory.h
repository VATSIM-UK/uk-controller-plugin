#pragma once
#include "handoff/HandoffCollection.h"

namespace UKControllerPlugin::Controller {
    class ControllerPositionHierarchyFactory;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Handoff {

    [[nodiscard]] auto Create(
        const Controller::ControllerPositionHierarchyFactory& controllerFactory,
        const nlohmann::json& handoffs) -> std::shared_ptr<HandoffCollection>;

    [[nodiscard]] auto HandoffOrderValid(
        const nlohmann::json& order, const Controller::ControllerPositionHierarchyFactory& controllerFactory)
        -> bool;
} // namespace UKControllerPlugin::Handoff

#pragma once
#include "handoff/HandoffCollection.h"

namespace UKControllerPlugin::Controller {
    class ControllerPositionCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Handoff {

    [[nodiscard]] auto Create(
        const UKControllerPlugin::Controller::ControllerPositionCollection& controllerPositions,
        const nlohmann::json& handoffs) -> std::unique_ptr<HandoffCollection>;

    [[nodiscard]] auto HandoffOrderValid(
        const nlohmann::json& order, const UKControllerPlugin::Controller::ControllerPositionCollection& controllerPositions)
        -> bool;
} // namespace UKControllerPlugin::Handoff

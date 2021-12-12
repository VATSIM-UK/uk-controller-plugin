#include "HandoffOrder.h"

namespace UKControllerPlugin::Handoff {
    HandoffOrder::HandoffOrder(int id, std::shared_ptr<Controller::ControllerPositionHierarchy> order)
        : id(id), order(std::move(order))
    {
    }
} // namespace UKControllerPlugin::Handoff

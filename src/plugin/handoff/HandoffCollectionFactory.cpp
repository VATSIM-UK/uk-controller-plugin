#include "HandoffCollectionFactory.h"
#include "HandoffOrder.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;

namespace UKControllerPlugin::Handoff {
    auto Create(const ControllerPositionHierarchyFactory& controllerFactory, const nlohmann::json& handoffs)
        -> std::shared_ptr<HandoffCollection>
    {
        std::unique_ptr<HandoffCollection> collection = std::make_unique<HandoffCollection>();

        if (!handoffs.is_array()) {
            LogError("JSON for handoff orders is not array");
            return std::move(collection);
        }

        for (const auto& handoff : handoffs) {
            if (!HandoffOrderValid(handoff, controllerFactory)) {
                LogWarning("Invalid handoff order detected");
                continue;
            }

            collection->Add(std::make_shared<HandoffOrder>(
                handoff.at("id").get<int>(),
                controllerFactory.CreateSharedFromJsonById(handoff.at("controller_positions"))));
        }

        LogInfo("Added " + std::to_string(collection->Count()) + " handoff orders");
        return std::move(collection);
    }

    /*
        Validate a handoff order
    */
    auto HandoffOrderValid(const nlohmann::json& order, const ControllerPositionHierarchyFactory& controllerFactory)
        -> bool
    {
        return order.is_object() && order.contains("id") && order.at("id").is_number_integer() &&
               order.contains("controller_positions") && order.at("controller_positions").is_array() &&
               !order.at("controller_positions").empty() &&
               controllerFactory.CreateSharedFromJsonById(order.at("controller_positions")) != nullptr;
    }
} // namespace UKControllerPlugin::Handoff

#include "HandoffCollectionFactory.h"
#include "HandoffOrder.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPlugin::Handoff {
    auto Create(const ControllerPositionCollection& controllerPositions, const nlohmann::json& handoffs)
        -> std::shared_ptr<HandoffCollection>
    {
        std::unique_ptr<HandoffCollection> collection = std::make_unique<HandoffCollection>();

        if (!handoffs.is_array()) {
            LogError("JSON for handoff orders is not array");
            return std::move(collection);
        }

        for (const auto& handoff : handoffs) {
            if (!HandoffOrderValid(handoff, controllerPositions)) {
                LogWarning("Invalid handoff order detected");
                continue;
            }

            // Build the hierarchy
            std::shared_ptr<ControllerPositionHierarchy> hierarchy = std::make_shared<ControllerPositionHierarchy>();
            for (const auto& controller : handoff.at("controller_positions")) {
                hierarchy->AddPosition(controllerPositions.FetchPositionById(controller.get<int>()));
            }

            collection->Add(std::make_shared<HandoffOrder>(handoff.at("id").get<int>(), hierarchy));
        }

        LogInfo("Added " + std::to_string(collection->Count()) + " handoff orders");
        return std::move(collection);
    }

    /*
        Validate a handoff order
    */
    auto HandoffOrderValid(const nlohmann::json& order, const ControllerPositionCollection& controllers) -> bool
    {
        return order.is_object() && order.contains("id") && order.at("id").is_number_integer() &&
               order.contains("key") && order.at("key").is_string() &&
               order.contains("controller_positions") && order.at("controller_positions").is_array() &&
               !order.at("controller_positions").empty() &&
               std::find_if_not(
                   order.at("controller_positions").cbegin(),
                   order.at("controller_positions").cend(),
                   [&controllers](const nlohmann::json& position) -> bool {
                       return position.is_number_integer() &&
                              controllers.FetchPositionById(position.get<int>()) != nullptr;
                   }) == order.at("controller_positions").cend();
    }
} // namespace UKControllerPlugin::Handoff

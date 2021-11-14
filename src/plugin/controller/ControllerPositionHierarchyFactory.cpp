#include "ControllerPosition.h"
#include "ControllerPositionCollection.h"
#include "ControllerPositionHierarchy.h"
#include "ControllerPositionHierarchyFactory.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPlugin::Controller {

    ControllerPositionHierarchyFactory::ControllerPositionHierarchyFactory(
        const ControllerPositionCollection& controllers)
        : controllers(controllers)
    {
    }

    /*
        Takes an array of callsigns in JSON form and converts these into a hierarchy of controllers.
    */
    auto ControllerPositionHierarchyFactory::CreateFromJsonByCallsign(const nlohmann::json& json) const
        -> std::unique_ptr<ControllerPositionHierarchy>
    {
        if (!json.is_array()) {
            LogError("Controller hierarchy is not an array: " + json.dump());
            throw std::invalid_argument("Json is not array");
        }

        std::unique_ptr<ControllerPositionHierarchy> hierarchy = std::make_unique<ControllerPositionHierarchy>();

        for (nlohmann::json::const_iterator it = json.cbegin(); it != json.cend(); ++it) {
            if (!it->is_string()) {
                throw std::invalid_argument("Controller in hierarchy not a string " + json.dump());
            }

            auto position = this->controllers.FetchPositionByCallsign(*it);
            if (!position) {
                throw std::invalid_argument("Controller position not found " + json.dump());
            }

            hierarchy->AddPosition(position);
        }

        return std::move(hierarchy);
    }

    std::unique_ptr<ControllerPositionHierarchy>
    ControllerPositionHierarchyFactory::CreateFromJsonById(const nlohmann::json& json) const
    {
        if (!json.is_array()) {
            LogError("Controller hierarchy is not an array: " + json.dump());
            return nullptr;
        }

        std::unique_ptr<ControllerPositionHierarchy> hierarchy = std::make_unique<ControllerPositionHierarchy>();
        for (const auto& position : json) {
            if (!position.is_number_integer()) {
                LogError("Controller position in hierarchy is not an integer");
                return nullptr;
            }

            auto resolvedPosition = this->controllers.FetchPositionById(position.get<int>());
            if (!resolvedPosition) {
                LogError("Controller position in hierarchy does not exist " + json.dump());
                return nullptr;
            }

            hierarchy->AddPosition(resolvedPosition);
        }

        return std::move(hierarchy);
    }
} // namespace UKControllerPlugin::Controller

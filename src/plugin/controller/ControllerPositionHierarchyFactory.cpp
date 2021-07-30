#include "pch/stdafx.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin {
    namespace Controller {

        ControllerPositionHierarchyFactory::ControllerPositionHierarchyFactory(
            const ControllerPositionCollection & controllers
        )
            : controllers(controllers)
        {

        }

        /*
            Takes an array of callsigns in JSON form and converts these into a hierarchy of controllers.
        */
        std::unique_ptr<ControllerPositionHierarchy> ControllerPositionHierarchyFactory::CreateFromJson(
            const nlohmann::json & json
        ) const
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

                hierarchy->AddPosition(this->controllers.FetchPositionByCallsign(*it));
            }

            return std::move(hierarchy);
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin

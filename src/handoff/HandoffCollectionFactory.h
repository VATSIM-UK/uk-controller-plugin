#include "handoff/HandoffCollection.h"
#include "controller/ControllerPositionCollection.h"

namespace UKControllerPlugin {
    namespace Handoff {

        std::unique_ptr<HandoffCollection> Create(
            const UKControllerPlugin::Controller::ControllerPositionCollection & controllerPositions,
            nlohmann::json handoffs,
            nlohmann::json sidMappings
        );

        bool HandoffOrderValid(
            nlohmann::json order,
            const UKControllerPlugin::Controller::ControllerPositionCollection& controllerPositions
        );
        bool SidMappingValid(nlohmann::json mapping);
    }  // namespace Handoff
}  // namespace UKControllerPlugin

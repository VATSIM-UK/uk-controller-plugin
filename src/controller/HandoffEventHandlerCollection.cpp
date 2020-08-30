#include "pch/stdafx.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "euroscope/EuroScopeCControllerInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPlugin {
    namespace Controller {

        size_t HandoffEventHandlerCollection::CountHandlers(void) const
        {
            return this->eventHandlers.size();
        }

        /*
            Adds an event handler to the collection.
        */
        void HandoffEventHandlerCollection::RegisterHandler(
            std::shared_ptr<HandoffEventHandlerInterface> handler
        ) {
            if (!this->eventHandlers.insert(handler).second) {
                LogWarning("Duplicate handoff event handler added");
            }
        }

        /*
            A handoff is initiated, propagate the event
        */
        void HandoffEventHandlerCollection::HandoffInitiated(
            EuroScopeCFlightPlanInterface& flightplan,
            EuroScopeCControllerInterface& transferringController,
            EuroScopeCControllerInterface& targetController
        ) const {
            for (
                auto it =
                this->eventHandlers.begin();
                it != this->eventHandlers.end();
                ++it
            ) {
                (*it)->HandoffInitiated(flightplan, transferringController, targetController);
            }
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin

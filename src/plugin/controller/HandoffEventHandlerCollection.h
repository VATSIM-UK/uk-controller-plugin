#pragma once
#include "controller/HandoffEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Controller {

        /*
            A class that stores a collection of HandoffEventHandlers
            and will call the appropriate handler when an event is triggered.
        */
        class HandoffEventHandlerCollection
        {
            public:
            void HandoffInitiated(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& transferringController,
                UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& targetController) const;
            size_t CountHandlers(void) const;
            void RegisterHandler(std::shared_ptr<UKControllerPlugin::Controller::HandoffEventHandlerInterface> handler);

            private:
            std::set<std::shared_ptr<UKControllerPlugin::Controller::HandoffEventHandlerInterface>> eventHandlers;
        };
    }; // namespace Controller
};     // namespace UKControllerPlugin

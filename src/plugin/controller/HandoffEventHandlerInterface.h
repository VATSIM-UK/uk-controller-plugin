#pragma once
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCControllerInterface.h"

namespace UKControllerPlugin {
    namespace Controller {

        /*
            An interface for handling events related to handoffs between controllers
        */
        class HandoffEventHandlerInterface
        {
            public:
                virtual void HandoffInitiated(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& transferringController,
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& targetController
                ) = 0;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin

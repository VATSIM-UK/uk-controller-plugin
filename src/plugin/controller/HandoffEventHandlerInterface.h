#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
    class EuroScopeCControllerInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Controller {

    /*
        An interface for handling events related to handoffs between controllers
    */
    class HandoffEventHandlerInterface
    {
        public:
        HandoffEventHandlerInterface();
        virtual ~HandoffEventHandlerInterface();
        HandoffEventHandlerInterface(const HandoffEventHandlerInterface&);
        HandoffEventHandlerInterface(HandoffEventHandlerInterface&&) = delete;
        [[nodiscard]] auto operator=(const HandoffEventHandlerInterface&) -> HandoffEventHandlerInterface&;
        [[nodiscard]] auto operator=(HandoffEventHandlerInterface&&) -> HandoffEventHandlerInterface& = delete;
        virtual void HandoffInitiated(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& transferringController,
            UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& targetController) = 0;
    };
} // namespace UKControllerPlugin::Controller

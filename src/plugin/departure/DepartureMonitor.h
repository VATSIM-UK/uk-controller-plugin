#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "message/UserMessager.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Controller {
        class Login;
    } // namespace Controller
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Departure {

    class DepartureMonitor : public TimedEvent::AbstractTimedEvent, public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        DepartureMonitor(const Controller::Login& login, Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void TimedEventTrigger() override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightplan) override;
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightplan, int dataType) override;
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;

        private:
        [[nodiscard]] auto HasDeparted(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const -> bool;

        // For checking controller logins
        const Controller::Login& login;

        // Plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Already departed
        std::map<std::string, std::string> alreadyDeparted;
    };
} // namespace UKControllerPlugin::Departure

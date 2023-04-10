#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "message/UserMessager.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
    class EuroScopeCRadarTargetInterface;
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Departure {

    class DepartureMonitor : public TimedEvent::AbstractTimedEvent, public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        DepartureMonitor(Euroscope::EuroscopePluginLoopbackInterface& plugin);
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

        // Plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Already departed
        std::map<std::string, std::string> alreadyDeparted;
    };
} // namespace UKControllerPlugin::Departure

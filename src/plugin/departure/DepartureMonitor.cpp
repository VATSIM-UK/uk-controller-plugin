#include "AircraftDepartedEvent.h"
#include "DepartureMonitor.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "eventhandler/EventBus.h"
#include "login/Login.h"

namespace UKControllerPlugin::Departure {

    DepartureMonitor::DepartureMonitor(
        const Controller::Login& login, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : login(login), plugin(plugin)
    {
    }

    void DepartureMonitor::TimedEventTrigger()
    {
        // Not logged in long enough
        if (login.GetSecondsLoggedIn() < std::chrono::seconds(10)) {
            LogInfo("Skipping departure monitor check as only just logged in");
            return;
        }

        plugin.ApplyFunctionToAllFlightplans([this](
                                                 std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface> fp,
                                                 std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface> rt) {
            // You only depart once
            if (alreadyDeparted.contains(fp->GetCallsign())) {
                return;
            }

            if (HasDeparted(*fp, *rt)) {
                alreadyDeparted.insert({fp->GetCallsign(), fp->GetOrigin()});
                LogDebug("Firing AircraftDepartedEvent for " + fp->GetCallsign() + " at " + fp->GetOrigin());
                UKControllerPluginUtils::EventHandler::EventBus::Bus().OnEvent<AircraftDepartedEvent>(
                    {fp->GetCallsign(), fp->GetOrigin()});
            }
        });
    }

    auto DepartureMonitor::HasDeparted(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const -> bool
    {
        // Workaround for ES putting aircraft at right on their destination during out-of-range handoff events
        if (radarTarget.GetFlightLevel() == 0 || flightplan.GetDistanceFromOrigin() == 0.0) {
            return false;
        }

        return !flightplan.GetOrigin().empty() && flightplan.GetDistanceFromOrigin() < 5.0 &&
               radarTarget.GetFlightLevel() < 5000 && radarTarget.GetFlightLevel() > 1500 &&
               radarTarget.GetGroundSpeed() > 70;
    }

    void DepartureMonitor::FlightPlanEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
    {
        if (alreadyDeparted.contains(flightplan.GetCallsign()) &&
            alreadyDeparted.at(flightplan.GetCallsign()) != flightplan.GetOrigin()) {
            alreadyDeparted.erase(flightplan.GetCallsign());
        }
    }

    void DepartureMonitor::FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        alreadyDeparted.erase(flightplan.GetCallsign());
    }

    void
    DepartureMonitor::ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightplan, int dataType)
    {
        // No-op
    }
} // namespace UKControllerPlugin::Departure

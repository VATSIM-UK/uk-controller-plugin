#include "pch/stdafx.h"
#include "historytrail/HistoryTrailEventHandler.h"
#include "historytrail/HistoryTrailRepository.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "historytrail/AircraftHistoryTrail.h"

using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPlugin::HistoryTrail::AircraftHistoryTrail;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace HistoryTrail {

        HistoryTrailEventHandler::HistoryTrailEventHandler(HistoryTrailRepository & repository)
            : repository(repository)
        {

        }

        /*
            Respond to a change in the controller flightplan data. At the moment, we don't have anything we
            need to do here.
        */
        void HistoryTrailEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {
        }

        /*
            Respond to a flightplan disconnection event.
        */
        void HistoryTrailEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->repository.UnregisterAircraft(flightPlan.GetCallsign());
        }

        /*
            Respond to a flightplan update event. Nothing to do here yet.
        */
        void HistoryTrailEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {

        }


        void HistoryTrailEventHandler::RadarTargetPositionUpdateEvent(EuroScopeCRadarTargetInterface & radarTarget)
        {
            // Register the aircraft with the history trail repository if not known.
            if (!this->repository.HasAircraft(radarTarget.GetCallsign())) {
                this->repository.RegisterAircraft(std::make_shared<AircraftHistoryTrail>(radarTarget.GetCallsign()));
            }

            // Add a point to the repo.
            this->repository.GetAircraft(radarTarget.GetCallsign())->AddItem(radarTarget.GetPosition());
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin

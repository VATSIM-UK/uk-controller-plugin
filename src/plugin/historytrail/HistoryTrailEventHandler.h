#pragma once
#include "euroscope/RadarTargetEventHandlerInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

// Forward Declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCRadarTargetInterface;
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
    namespace HistoryTrail {
        class HistoryTrailRepository;
    } // namespace HistoryTrail
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace HistoryTrail {

        /*
            Class that handles events in relation to history trails.
        */
        class HistoryTrailEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
                                         public UKControllerPlugin::Euroscope::RadarTargetEventHandlerInterface
        {
            public:
            explicit HistoryTrailEventHandler(UKControllerPlugin::HistoryTrail::HistoryTrailRepository& repository);
            void ControllerFlightPlanDataEvent(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
            void FlightPlanDisconnectEvent(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
            void FlightPlanEvent(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
            void RadarTargetPositionUpdateEvent(
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;

            private:
            // History trail repository that events will be passed to.
            UKControllerPlugin::HistoryTrail::HistoryTrailRepository& repository;
        };
    } // namespace HistoryTrail
} // namespace UKControllerPlugin

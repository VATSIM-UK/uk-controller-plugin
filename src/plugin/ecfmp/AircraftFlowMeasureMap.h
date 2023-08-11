#pragma once
#include <string>
#include <unordered_set>
#include "ECFMP/SdkEvents.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include "AircraftFlowMeasureMapInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::ECFMP {
    class AircraftFlowMeasureMap : public ::ECFMP::EventBus::EventListener<::ECFMP::Plugin::FlowMeasureActivatedEvent>,
                                   public ::ECFMP::EventBus::EventListener<::ECFMP::Plugin::FlowMeasureExpiredEvent>,
                                   public ::ECFMP::EventBus::EventListener<::ECFMP::Plugin::FlowMeasureWithdrawnEvent>,
                                   public Flightplan::FlightPlanEventHandlerInterface,
                                   public AircraftFlowMeasureMapInterface
    {
        public:
        AircraftFlowMeasureMap(Euroscope::EuroscopePluginLoopbackInterface& plugin);
        ~AircraftFlowMeasureMap() override;
        void FlightPlanEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void
        FlightPlanDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        void OnEvent(const ::ECFMP::Plugin::FlowMeasureActivatedEvent& event) override;
        void OnEvent(const ::ECFMP::Plugin::FlowMeasureExpiredEvent& event) override;
        void OnEvent(const ::ECFMP::Plugin::FlowMeasureWithdrawnEvent& event) override;
        auto GetFlowMeasuresForCallsign(const std::string& callsign)
            -> const std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>& override;

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
} // namespace UKControllerPlugin::ECFMP

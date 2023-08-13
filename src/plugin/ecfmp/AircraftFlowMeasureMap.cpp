#include "AircraftFlowMeasureMap.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::ECFMP {

    struct AircraftFlowMeasureMap::Impl
    {
        Impl(Euroscope::EuroscopePluginLoopbackInterface& plugin) : plugin(plugin)
        {
        }

        void RemoveFlightplanFromCollection(Euroscope::EuroScopeCFlightPlanInterface& flightplan)
        {
            // No flow measures are relevant to this flight plan
            if (!callsignFlowMeasureMap.contains(flightplan.GetCallsign())) {
                return;
            }

            // Remove the flight plan from the flow measure's callsign map
            for (const auto& flowMeasure : callsignFlowMeasureMap.at(flightplan.GetCallsign())) {
                flowMeasureCallsignMap.at(flowMeasure).erase(flightplan.GetCallsign());
            }

            // Remove the callsign from the callsign map
            callsignFlowMeasureMap.erase(flightplan.GetCallsign());
        }

        void RemoveFlowMeasureFromCollection(const ::ECFMP::FlowMeasure::FlowMeasure& measure)
        {
            // Measure never became active
            if (!flowMeasureIdMap.contains(measure.Id())) {
                return;
            }

            const auto collectionMeasure = flowMeasureIdMap.at(measure.Id());

            // Remove from the callsign map
            for (const auto& callsign : flowMeasureCallsignMap.at(collectionMeasure)) {
                callsignFlowMeasureMap.at(callsign).erase(collectionMeasure);
            }

            // Remove from the map by measure and id
            flowMeasureCallsignMap.erase(collectionMeasure);
            flowMeasureIdMap.erase(measure.Id());
        }

        void FlowMeasureActivated(const std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>& measure)
        {
            if (flowMeasureIdMap.contains(measure->Id())) {
                LogWarning("Flow measure with id " + std::to_string(measure->Id()) + " already exists in the map");
                return;
            }

            // Add the flow measure to the map by id
            flowMeasureIdMap.emplace(measure->Id(), measure);

            // Check each of the flightplans and see if it applies to them
            plugin.ApplyFunctionToAllFlightplans([&measure, this](
                                                     const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                                                     const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) {
                if (measure->ApplicableToAircraft(flightplan.GetEuroScopeObject(), radarTarget.GetEuroScopeObject())) {
                    callsignFlowMeasureMap[flightplan.GetCallsign()].insert(measure);
                    flowMeasureCallsignMap[measure].insert(flightplan.GetCallsign());
                }
            });
        }

        void MapFlightplanToActiveFlowMeasures(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        {
            for (const auto& [id, measure] : flowMeasureIdMap) {
                if (measure->ApplicableToAircraft(flightPlan.GetEuroScopeObject(), radarTarget.GetEuroScopeObject())) {
                    callsignFlowMeasureMap[flightPlan.GetCallsign()].insert(measure);
                    flowMeasureCallsignMap[measure].insert(flightPlan.GetCallsign());
                }
            }
        }

        // The plugin, used to map over flightplans
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Map of callsigns to flow measures and vice versa
        std::map<int, std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>> flowMeasureIdMap;
        std::map<std::string, std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>>
            callsignFlowMeasureMap;
        std::map<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>, std::unordered_set<std::string>>
            flowMeasureCallsignMap;

        // An empty set for use when we dont have a callsign
        std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>> emptyFlowMeasureSet;
    };

    AircraftFlowMeasureMap::AircraftFlowMeasureMap(Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : impl(std::make_unique<Impl>(plugin))
    {
    }

    AircraftFlowMeasureMap::~AircraftFlowMeasureMap() = default;

    void AircraftFlowMeasureMap::OnEvent(const ::ECFMP::Plugin::FlowMeasureActivatedEvent& event)
    {
        impl->FlowMeasureActivated(event.flowMeasure);
    }

    void AircraftFlowMeasureMap::OnEvent(const ::ECFMP::Plugin::FlowMeasureExpiredEvent& event)
    {
        impl->RemoveFlowMeasureFromCollection(*event.flowMeasure);
    }

    void AircraftFlowMeasureMap::OnEvent(const ::ECFMP::Plugin::FlowMeasureWithdrawnEvent& event)
    {
        impl->RemoveFlowMeasureFromCollection(*event.flowMeasure);
    }

    void AircraftFlowMeasureMap::FlightPlanEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
    {
        impl->RemoveFlightplanFromCollection(flightPlan);
        impl->MapFlightplanToActiveFlowMeasures(flightPlan, radarTarget);
    }

    void AircraftFlowMeasureMap::FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan)
    {
        impl->RemoveFlightplanFromCollection(flightPlan);
    }

    const std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>&
    AircraftFlowMeasureMap::GetFlowMeasuresForCallsign(const std::string& callsign) const
    {
        return impl->callsignFlowMeasureMap.contains(callsign) ? impl->callsignFlowMeasureMap.at(callsign)
                                                               : impl->emptyFlowMeasureSet;
    }

    void AircraftFlowMeasureMap::ControllerFlightPlanDataEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        // No-op
    }
} // namespace UKControllerPlugin::ECFMP

#pragma once

namespace UKControllerPlugin::ECFMP {
    class AircraftFlowMeasureMapInterface
    {
        public:
        virtual ~AircraftFlowMeasureMapInterface() = default;
        [[nodiscard]] virtual auto GetFlowMeasuresForCallsign(const std::string& callsign)
            -> const std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>& = 0;
    };
} // namespace UKControllerPlugin::ECFMP

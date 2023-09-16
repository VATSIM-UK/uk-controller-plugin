#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
    class EuroScopeCRadarTargetInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::ECFMP {
    class ECFMPCustomMeasureFilter
    {
        public:
        virtual ~ECFMPCustomMeasureFilter() noexcept = default;
        [[nodiscard]] virtual auto FlowMeasureApplicableToAircraft(
            const Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const ::ECFMP::FlowMeasure::FlowMeasure& flowMeasure) const noexcept -> bool = 0;
    };
} // namespace UKControllerPlugin::ECFMP

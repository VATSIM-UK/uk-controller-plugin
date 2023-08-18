#pragma once
#include "ECFMPCustomMeasureFilter.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"

namespace UKControllerPlugin::Controller {
    class ActiveCallsignCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::ECFMP {

    /**
     * Checks if a flow measure is relevant to the controller.
     */
    class ControllerFlowMeasureRelevance : public ECFMPCustomMeasureFilter
    {
        public:
        explicit ControllerFlowMeasureRelevance(const Controller::ActiveCallsignCollection& activeCallsigns) noexcept;
        ~ControllerFlowMeasureRelevance() noexcept override;
        auto FlowMeasureApplicableToAircraft(
            const Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const ::ECFMP::FlowMeasure::FlowMeasure& flowMeasure) const noexcept -> bool override;

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

} // namespace UKControllerPlugin::ECFMP

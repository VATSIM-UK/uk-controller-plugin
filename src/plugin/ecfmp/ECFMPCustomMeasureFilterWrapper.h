#pragma once

namespace UKControllerPlugin::ECFMP {

    class ECFMPCustomMeasureFilter;

    /**
     * Wraps an ECFMPCustomMeasureFilter instance so that we can take the raw EuroScope objects
     * and turn them into our own interfaces
     */
    class ECFMPCustomMeasureFilterWrapper : public ::ECFMP::FlowMeasure::CustomFlowMeasureFilter
    {
        public:
        explicit ECFMPCustomMeasureFilterWrapper(std::shared_ptr<ECFMPCustomMeasureFilter> wrappedFilter) noexcept;

        [[nodiscard]] auto ApplicableToAircraft(
            const EuroScopePlugIn::CFlightPlan& flightplan,
            const EuroScopePlugIn::CRadarTarget& radarTarget,
            const ::ECFMP::FlowMeasure::FlowMeasure& flowMeasure) const noexcept -> bool override;

        private:
        std::shared_ptr<ECFMPCustomMeasureFilter> wrappedFilter;
    };

} // namespace UKControllerPlugin::ECFMP

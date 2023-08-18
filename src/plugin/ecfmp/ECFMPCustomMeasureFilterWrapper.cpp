#include "ECFMPCustomMeasureFilter.h"
#include "ECFMPCustomMeasureFilterWrapper.h"
#include "euroscope/EuroScopeCFlightPlanWrapper.h"
#include "euroscope/EuroScopeCRadarTargetWrapper.h"

namespace UKControllerPlugin::ECFMP {

    ECFMPCustomMeasureFilterWrapper::ECFMPCustomMeasureFilterWrapper(
        std::shared_ptr<ECFMPCustomMeasureFilter> wrappedFilter) noexcept
        : wrappedFilter(std::move(wrappedFilter))
    {
        assert(this->wrappedFilter != nullptr && "Wrapped filter cannot be null");
    }

    auto ECFMPCustomMeasureFilterWrapper::ApplicableToAircraft(
        const EuroScopePlugIn::CFlightPlan& flightplan,
        const EuroScopePlugIn::CRadarTarget& radarTarget,
        const ::ECFMP::FlowMeasure::FlowMeasure& flowMeasure) const noexcept -> bool
    {
        return wrappedFilter->FlowMeasureApplicableToAircraft(
            Euroscope::EuroScopeCFlightPlanWrapper(flightplan),
            Euroscope::EuroScopeCRadarTargetWrapper(radarTarget),
            flowMeasure);
    }
} // namespace UKControllerPlugin::ECFMP

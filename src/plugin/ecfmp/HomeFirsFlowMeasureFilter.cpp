#include "HomeFirsFlowMeasureFilter.h"

namespace UKControllerPlugin::ECFMP {

    auto HomeFirsFlowMeasureFilter::ShouldProcess(const ::ECFMP::Plugin::FlowMeasureActivatedEvent& event) -> bool
    {
        return IsNotifiedToHomeFirs(*event.flowMeasure);
    }

    auto HomeFirsFlowMeasureFilter::ShouldProcess(const ::ECFMP::Plugin::FlowMeasureNotifiedEvent& event) -> bool
    {
        return IsNotifiedToHomeFirs(*event.flowMeasure);
    }

    auto HomeFirsFlowMeasureFilter::ShouldProcess(const ::ECFMP::Plugin::FlowMeasureExpiredEvent& event) -> bool
    {
        return IsNotifiedToHomeFirs(*event.flowMeasure);
    }

    auto HomeFirsFlowMeasureFilter::ShouldProcess(const ::ECFMP::Plugin::FlowMeasureWithdrawnEvent& event) -> bool
    {
        return IsNotifiedToHomeFirs(*event.flowMeasure);
    }

    auto HomeFirsFlowMeasureFilter::ShouldProcess(const ::ECFMP::Plugin::FlowMeasureReissuedEvent& event) -> bool
    {
        return IsNotifiedToHomeFirs(*event.original);
    }

    auto HomeFirsFlowMeasureFilter::IsNotifiedToHomeFirs(const ::ECFMP::FlowMeasure::FlowMeasure& measure) -> bool
    {
        const auto notifiedRegions = measure.NotifiedFlightInformationRegions();
        return std::find_if(notifiedRegions.cbegin(), notifiedRegions.cend(), ([](const auto& region) {
                                return region->Identifier() == "EGTT" || region->Identifier() == "EGPX";
                            })) != notifiedRegions.cend();
    }
} // namespace UKControllerPlugin::ECFMP

#include "HomeFirsFlowMeasureFilter.h"
#include <algorithm>

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

        // XXXX is the identifier used in ECFMP for the global / catch all FIR
        return std::any_of(notifiedRegions.cbegin(), notifiedRegions.cend(), ([](const auto& region) {
                               return region->Identifier() == "EGTT" || region->Identifier() == "EGPX" ||
                                      region->Identifier() == "XXXX";
                           }));
    }
} // namespace UKControllerPlugin::ECFMP

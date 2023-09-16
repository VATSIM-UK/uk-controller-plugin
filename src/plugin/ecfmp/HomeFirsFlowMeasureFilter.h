#pragma once

#include "ECFMP/flowmeasure/FlowMeasure.h"
namespace UKControllerPlugin::ECFMP {

    class HomeFirsFlowMeasureFilter : public ::ECFMP::EventBus::EventFilter<::ECFMP::Plugin::FlowMeasureActivatedEvent>,
                                      public ::ECFMP::EventBus::EventFilter<::ECFMP::Plugin::FlowMeasureNotifiedEvent>,
                                      public ::ECFMP::EventBus::EventFilter<::ECFMP::Plugin::FlowMeasureExpiredEvent>,
                                      public ::ECFMP::EventBus::EventFilter<::ECFMP::Plugin::FlowMeasureWithdrawnEvent>,
                                      public ::ECFMP::EventBus::EventFilter<::ECFMP::Plugin::FlowMeasureReissuedEvent>
    {
        public:
        [[nodiscard]] auto ShouldProcess(const ::ECFMP::Plugin::FlowMeasureActivatedEvent& event) -> bool override;
        [[nodiscard]] auto ShouldProcess(const ::ECFMP::Plugin::FlowMeasureNotifiedEvent& event) -> bool override;
        [[nodiscard]] auto ShouldProcess(const ::ECFMP::Plugin::FlowMeasureExpiredEvent& event) -> bool override;
        [[nodiscard]] auto ShouldProcess(const ::ECFMP::Plugin::FlowMeasureWithdrawnEvent& event) -> bool override;
        [[nodiscard]] auto ShouldProcess(const ::ECFMP::Plugin::FlowMeasureReissuedEvent& event) -> bool override;

        private:
        [[nodiscard]] static auto IsNotifiedToHomeFirs(const ::ECFMP::FlowMeasure::FlowMeasure& measure) -> bool;
    };
} // namespace UKControllerPlugin::ECFMP

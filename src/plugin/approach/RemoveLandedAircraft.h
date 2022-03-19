#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;

    /**
     * Removes aircraft that have landed from the approach sequencer
     */
    class RemoveLandedAircraft : public TimedEvent::AbstractTimedEvent
    {
        public:
        RemoveLandedAircraft(ApproachSequencer& sequencer, Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void TimedEventTrigger() override;

        private:
        
        // Manages approach sequnces
        ApproachSequencer& sequencer;
        
        // For getting flightplans and radar targets
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Approach

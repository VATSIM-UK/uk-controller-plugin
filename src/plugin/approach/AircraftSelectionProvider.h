#pragma once
#include "aircraft/CallsignSelectionProviderInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;
    class ApproachSequencerDisplayOptions;

    class AircraftSelectionProvider : public Aircraft::CallsignSelectionProviderInterface
    {
        public:
        AircraftSelectionProvider(
            ApproachSequencer& sequencer,
            std::shared_ptr<ApproachSequencerDisplayOptions> options,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        auto GetCallsigns() -> std::set<std::string> override;
        void CallsignSelected(const std::string& callsign) override;

        private:
        // For sequencing approaches
        ApproachSequencer& sequencer;

        // Display options
        std::shared_ptr<ApproachSequencerDisplayOptions> options;

        // For flightplan iteration
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Approach

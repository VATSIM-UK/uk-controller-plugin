#include "ApproachSequence.h"
#include "ApproachSequencer.h"
#include "RemoveLandedAircraft.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Approach {

    RemoveLandedAircraft::RemoveLandedAircraft(
        ApproachSequencer& sequencer, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : sequencer(sequencer), plugin(plugin)
    {
    }

    void RemoveLandedAircraft::TimedEventTrigger()
    {
        sequencer.ForEach([this](const ApproachSequence& sequence) {
            const auto callsigns = sequence.Callsigns();
            for (const auto& callsign : callsigns) {
                const auto radarTarget = plugin.GetRadarTargetForCallsign(callsign);
                if (radarTarget && radarTarget->GetFlightLevel() < 1000 && radarTarget->GetGroundSpeed() < 60) {
                    sequencer.RemoveAircraftFromSequences(callsign);
                }
            }
        });
    }
} // namespace UKControllerPlugin::Approach

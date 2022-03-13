#include "AircraftSelectionProvider.h"
#include "ApproachSequence.h"
#include "ApproachSequencer.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSequencingMode.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Approach {

    AircraftSelectionProvider::AircraftSelectionProvider(
        ApproachSequencer& sequencer,
        std::shared_ptr<ApproachSequencerDisplayOptions> options,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : sequencer(sequencer), options(std::move(options)), plugin(plugin)
    {
    }

    auto AircraftSelectionProvider::GetCallsigns() -> std::set<std::string>
    {
        if (options->Airfield().empty()) {
            return {};
        }

        const auto& sequence = sequencer.GetForAirfield(options->Airfield());
        std::set<std::string> callsigns;
        this->plugin.ApplyFunctionToAllFlightplans(
            [&callsigns, &sequence, this](
                const std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface>& fp,
                const std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface>& rt) {
                if (fp->GetDestination() != options->Airfield() || sequence.Get(fp->GetCallsign()) != nullptr ||
                    rt->GetGroundSpeed() < 50) {
                    return;
                }

                callsigns.insert(fp->GetCallsign());
            });

        return callsigns;
    }

    void AircraftSelectionProvider::CallsignSelected(const std::string& callsign)
    {
        if (options->Airfield().empty()) {
            return;
        }

        this->sequencer.RemoveAircraftFromSequences(callsign);
        this->sequencer.AddAircraftToSequence(options->Airfield(), callsign, ApproachSequencingMode::WakeTurbulence);
    }
} // namespace UKControllerPlugin::Approach

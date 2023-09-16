#include "ControllerFlowMeasureRelevance.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include <algorithm>

namespace UKControllerPlugin::ECFMP {

    struct ControllerFlowMeasureRelevance::Impl
    {
        [[nodiscard]] auto UserIsScottishController() const noexcept -> bool
        {
            const auto& userCallsign = activeCallsigns.GetUserCallsign();
            const auto normalisedCallsign = userCallsign.GetNormalisedPosition().GetCallsign();

            return normalisedCallsign.substr(0, 3) == "SCO" || normalisedCallsign.substr(0, 3) == "EGP" ||
                   normalisedCallsign.substr(0, 3) == "STC" || normalisedCallsign.substr(0, 3) == "EGA" ||
                   normalisedCallsign.substr(0, 3) == "EGQ";
        }

        [[nodiscard]] auto UserIsLondonController() const noexcept -> bool
        {
            const auto& userCallsign = activeCallsigns.GetUserCallsign();
            const auto normalisedCallsign = userCallsign.GetNormalisedPosition().GetCallsign();

            return (normalisedCallsign.substr(0, 2) == "EG" && normalisedCallsign.substr(0, 3) != "EGP" &&
                    normalisedCallsign.substr(0, 3) != "EGQ" && normalisedCallsign.substr(0, 3) != "EGA") ||
                   normalisedCallsign.substr(0, 3) == "LON" || normalisedCallsign.substr(0, 3) == "LTC" ||
                   normalisedCallsign.substr(0, 3) == "MAN" || normalisedCallsign.substr(0, 5) == "ESSEX" ||
                   normalisedCallsign.substr(0, 6) == "THAMES" || normalisedCallsign.substr(0, 6) == "SOLENT";
        }

        // The active callsigns for the controller
        const Controller::ActiveCallsignCollection& activeCallsigns;
    };

    ControllerFlowMeasureRelevance::ControllerFlowMeasureRelevance(
        const Controller::ActiveCallsignCollection& activeCallsigns) noexcept
        : impl(std::make_unique<Impl>(Impl{activeCallsigns}))
    {
    }

    ControllerFlowMeasureRelevance::~ControllerFlowMeasureRelevance() noexcept = default;

    auto ControllerFlowMeasureRelevance::FlowMeasureApplicableToAircraft(
        const Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
        const ::ECFMP::FlowMeasure::FlowMeasure& flowMeasure) const noexcept -> bool
    {
        // If the user isn't an active callsign, just show everything
        if (!impl->activeCallsigns.UserHasCallsign()) {
            return true;
        }

        // If the user is an active callsign, only show flow measures that are relevant to them
        return std::any_of(
            flowMeasure.NotifiedFlightInformationRegions().cbegin(),
            flowMeasure.NotifiedFlightInformationRegions().cend(),
            [this](const auto& fir) {
                return fir->Identifier() == "XXXX" ||
                       (fir->Identifier() == "EGPX" && impl->UserIsScottishController()) ||
                       (fir->Identifier() == "EGTT" && impl->UserIsLondonController());
            });
    }
} // namespace UKControllerPlugin::ECFMP

#include "AirfieldApproachOptions.h"
#include "ApproachSequencedAircraft.h"
#include "ApproachSequencerOptions.h"
#include "ApproachSpacingCalculator.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "wake/ArrivalWakeInterval.h"
#include "wake/WakeCategory.h"
#include "wake/WakeCategoryMapperCollection.h"
#include "wake/WakeCategoryMapperInterface.h"

namespace UKControllerPlugin::Approach {

    ApproachSpacingCalculator::ApproachSpacingCalculator(
        ApproachSequencerOptions& sequencerOptions,
        const Airfield::AirfieldCollection& airfields,
        const Wake::WakeCategoryMapperCollection& wakeMappers,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : sequencerOptions(sequencerOptions), airfields(airfields), wakeMappers(wakeMappers), plugin(plugin)
    {
    }

    auto
    ApproachSpacingCalculator::Calculate(const std::string& airfield, const ApproachSequencedAircraft& aircraft) const
        -> double
    {
        /*
         * Start by checking that we have all the information we need in order to be able to make a
         * calculation.
         */
        if (!aircraft.Previous()) {
            return NoSpacing();
        }

        auto airfieldModel = airfields.FetchAirfieldByIcao(airfield);
        if (!airfieldModel) {
            return NoSpacing();
        }

        const auto thisFlightplan = plugin.GetFlightplanForCallsign(aircraft.Callsign());
        const auto previousFlightplan = plugin.GetFlightplanForCallsign(aircraft.Previous()->Callsign());
        if (!thisFlightplan || !previousFlightplan) {
            return NoSpacing();
        }

        auto minimumSeparation = AirfieldMinimumSeparation(airfield);
        auto wakeMapper = wakeMappers.Get(airfieldModel->WakeScheme());
        if (!wakeMapper) {
            return minimumSeparation;
        }

        auto thisWakeCategory = wakeMapper->MapForFlightplan(*thisFlightplan);
        auto previousWakeCategory = wakeMapper->MapForFlightplan(*previousFlightplan);
        if (!thisWakeCategory || !previousWakeCategory) {
            return minimumSeparation;
        }

        auto requiredWakeDistance = previousWakeCategory->ArrivalInterval(*thisWakeCategory);

        // If the target is wake, return the required arrival interval
        if (aircraft.Mode() == ApproachSequencingMode::WakeTurbulence) {
            return requiredWakeDistance ? requiredWakeDistance->Value() : AirfieldMinimumSeparation(airfield);
        }

        // If we're in minimum distance mode, then take the larger of the two.
        return requiredWakeDistance && requiredWakeDistance->Value() > aircraft.ExpectedDistance()
                   ? GreaterOf(requiredWakeDistance->Value(), minimumSeparation)
                   : GreaterOf(aircraft.ExpectedDistance(), minimumSeparation);
    }

    auto ApproachSpacingCalculator::NoSpacing() -> double
    {
        return NO_SPACING_DETECTED;
    }

    auto ApproachSpacingCalculator::AirfieldMinimumSeparation(const std::string& airfield) const -> double
    {
        return sequencerOptions.Get(airfield).minimumSeparationRequirement;
    }

    auto ApproachSpacingCalculator::GreaterOf(double value1, double value2) -> double
    {
        return value1 > value2 ? value1 : value2;
    }
} // namespace UKControllerPlugin::Approach

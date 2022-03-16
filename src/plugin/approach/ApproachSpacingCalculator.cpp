#include "ApproachSequencedAircraft.h"
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
        const Airfield::AirfieldCollection& airfields,
        const Wake::WakeCategoryMapperCollection& wakeMappers,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : airfields(airfields), wakeMappers(wakeMappers), plugin(plugin)
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

        auto wakeMapper = wakeMappers.Get(airfieldModel->WakeScheme());
        if (!wakeMapper) {
            return NoSpacing();
        }

        const auto thisFlightplan = plugin.GetFlightplanForCallsign(aircraft.Callsign());
        const auto previousFlightplan = plugin.GetFlightplanForCallsign(aircraft.Previous()->Callsign());
        if (!thisFlightplan || !previousFlightplan) {
            return NoSpacing();
        }

        auto thisWakeCategory = wakeMapper->MapForFlightplan(*thisFlightplan);
        auto previousWakeCategory = wakeMapper->MapForFlightplan(*previousFlightplan);
        if (!thisWakeCategory || !previousWakeCategory) {
            return NoSpacing();
        }

        auto requiredWakeDistance = previousWakeCategory->ArrivalInterval(*thisWakeCategory);

        // If the target is wake, return the required arrival interval
        if (aircraft.Mode() == ApproachSequencingMode::WakeTurbulence) {
            return requiredWakeDistance ? requiredWakeDistance->Value() : NoSpacing();
        }

        // If we're in minimum distance mode, then take the larger of the two.
        return requiredWakeDistance && requiredWakeDistance->Value() > aircraft.ExpectedDistance()
                   ? requiredWakeDistance->Value()
                   : aircraft.ExpectedDistance();
    }

    auto ApproachSpacingCalculator::NoSpacing() -> double
    {
        return NO_SPACING_DETECTED;
    }
} // namespace UKControllerPlugin::Approach

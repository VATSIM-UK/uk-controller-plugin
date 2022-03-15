#include "ApproachSequencedAircraft.h"
#include "ApproachSpacingCalculator.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "wake/WakeCategoryMapperCollection.h"

namespace UKControllerPlugin::Approach {

    ApproachSpacingCalculator::ApproachSpacingCalculator(
        const Airfield::AirfieldCollection& airfields, const Wake::WakeCategoryMapperCollection& wakeMappers)
        : airfields(airfields), wakeMappers(wakeMappers)
    {
    }

    auto
    ApproachSpacingCalculator::Calculate(const std::string& airfield, const ApproachSequencedAircraft& aircraft) const
        -> double
    {
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
        
        if (aircraft.Mode() == ApproachSequencingMode::WakeTurbulence) {
            auto distance =
        }
    }

    auto ApproachSpacingCalculator::NoSpacing() -> double
    {
        return NO_SPACING_DETECTED;
    }
} // namespace UKControllerPlugin::Approach

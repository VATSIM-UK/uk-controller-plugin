#include "DistanceRadiusToScreenRadius.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"

namespace UKControllerPlugin::Geometry {

    auto
    ScreenRadiusFromDistance(double distanceInNauticalMiles, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
        -> double
    {
        auto viewport = radarScreen.GetRadarViewport();

        /**
         * The screen radius for the required distance is:
         *
         * Number of nautical miles we want
         *
         * Multiplied by
         *
         * The number of pixels per nautical mile on the viewport.
         */
        return distanceInNauticalMiles * ((viewport.bottom - viewport.top) /
                                          radarScreen.ConvertScreenPointToCoordinate({0, 0}).DistanceTo(
                                              radarScreen.ConvertScreenPointToCoordinate({0, viewport.bottom})));
    }
} // namespace UKControllerPlugin::Geometry

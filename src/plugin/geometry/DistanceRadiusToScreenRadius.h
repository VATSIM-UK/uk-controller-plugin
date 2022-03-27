#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroscopeRadarLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Geometry {
    [[nodiscard]] auto
    ScreenRadiusFromDistance(double distanceInNauticalMiles, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
        -> double;
} // namespace UKControllerPlugin::Geometry

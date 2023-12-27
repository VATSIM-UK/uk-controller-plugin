#include "Angle.h"

namespace UKControllerPlugin::Geometry {

    const double pi = 3.14159265358979323846;

    auto DegreesToRadians(const double degrees) -> double
    {
        return degrees * pi / 180;
    }

    auto RadiansToDegrees(const double radians) -> double
    {
        return radians * 180 / pi;
    }

    auto Slope(const double radians) -> double
    {
        return std::tan(radians);
    }
} // namespace UKControllerPlugin::Geometry

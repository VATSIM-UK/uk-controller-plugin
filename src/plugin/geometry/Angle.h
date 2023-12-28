#pragma once

namespace UKControllerPlugin::Geometry {
    [[nodiscard]] auto DegreesToRadians(const double degrees) -> double;
    [[nodiscard]] auto RadiansToDegrees(const double radians) -> double;
    [[nodiscard]] auto Slope(const double radians) -> double;

} // namespace UKControllerPlugin::Geometry

#pragma once

namespace UKControllerPlugin::Geometry {
    [[nodiscard]] inline auto NauticalMilesToFeet(double nauticalMiles) -> double
    {
        return nauticalMiles * 6076.115;
    }

    [[nodiscard]] inline auto FeetToNauticalMiles(double feet) -> double
    {
        return feet / 6076.115;
    }
} // namespace UKControllerPlugin::Geometry

#pragma once

namespace UKControllerPlugin::Headings {
    enum class Heading : unsigned int
    {
        North = 360,
        N = North,
        NorthEast = 45,
        NE = NorthEast,
        East = 90,
        E = East,
        SouthEast = 135,
        SE = SouthEast,
        South = 180,
        S = South,
        SouthWest = 225,
        SW = SouthWest,
        West = 270,
        W = West,
        NorthWest = 315,
        NW = NorthWest
    };

    auto operator<(Heading first, Heading second) -> bool;
    auto operator>=(Heading first, Heading second) -> bool;
    auto operator<(double first, Heading second) -> bool;
    auto operator>=(double first, Heading second) -> bool;
    auto operator==(unsigned int first, Heading second) -> bool;

    [[nodiscard]] auto TruncateHeading(unsigned int heading) -> unsigned int;
    [[nodiscard]] auto PerpendicularHeading(unsigned int heading) -> unsigned int;
} // namespace UKControllerPlugin::Headings

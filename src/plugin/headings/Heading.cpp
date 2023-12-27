#include "Heading.h"

namespace UKControllerPlugin::Headings {
    auto operator<(Heading first, Heading second) -> bool
    {
        return static_cast<unsigned int>(first) < static_cast<unsigned int>(second);
    }

    auto operator>=(Heading first, Heading second) -> bool
    {
        return static_cast<unsigned int>(first) >= static_cast<unsigned int>(second);
    }

    auto operator<(double first, Heading second) -> bool
    {
        return first < static_cast<unsigned int>(second);
    }

    auto operator>=(double first, Heading second) -> bool
    {
        return first >= static_cast<unsigned int>(second);
    }

    auto operator==(unsigned int first, Heading second) -> bool
    {
        return first == static_cast<unsigned int>(second);
    }

    [[nodiscard]] auto TruncateHeading(unsigned int heading) -> unsigned int
    {
        return heading % 360;
    }

    [[nodiscard]] auto PerpendicularHeading(unsigned int heading) -> unsigned int
    {
        return TruncateHeading(heading + 90);
    }
} // namespace UKControllerPlugin::Headings

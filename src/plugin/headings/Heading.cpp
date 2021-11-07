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
} // namespace UKControllerPlugin::Headings

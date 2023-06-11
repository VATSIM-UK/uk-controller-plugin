#include "SquawkCodeValid.h"

namespace UKControllerPlugin::Squawk {

    // Squawk regex
    const std::regex squawkRegex("^[0-7]{4}$");

    auto SquawkCodeValid(const std::string& squawk) -> bool
    {
        return std::regex_match(squawk, squawkRegex);
    }
} // namespace UKControllerPlugin::Squawk

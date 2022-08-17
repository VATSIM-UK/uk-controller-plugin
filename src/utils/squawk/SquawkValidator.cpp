#include "SquawkValidator.h"

namespace UKControllerPlugin::Squawk {

    /*
        Returns true iff the squawk is allowed to be assigned - e.g. we would not want
        to assign 7500.
    */
    auto SquawkValidator::AllowedSquawk(const std::string& squawk) -> bool
    {
        return squawk != "7500" && squawk != "7600" && squawk != "7700" && squawk != "1200" && squawk != "2200" &&
               squawk != "0200" && squawk != "0000" && squawk != "1234";
    }

    /*
        Returns true of the squawk is valid
    */
    auto SquawkValidator::ValidSquawk(std::string squawk) -> bool
    {
        return squawk.size() == 4 && std::find_if(squawk.begin(), squawk.end(), [](char c) {
                                         return !std::isdigit(c) || c == '8' || c == '9';
                                     }) == squawk.end();
    }
} // namespace UKControllerPlugin::Squawk

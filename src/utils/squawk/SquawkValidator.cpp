#include "pch/pch.h"
#include "squawk/SquawkValidator.h"

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Returns true iff the squawk is allowed to be assigned - e.g. we would not want
            to assign 7500.
        */
        bool SquawkValidator::AllowedSquawk(std::string squawk)
        {
            return squawk != "7500" &&
                squawk != "7600" &&
                squawk != "7700" &&
                squawk != "1200" &&
                squawk != "2200" &&
                squawk != "0200";
        }

        /*
            Returns true of the squawk is valid
        */
        bool SquawkValidator::ValidSquawk(std::string squawk)
        {
            return squawk.size() == 4 && std::find_if(squawk.begin(),
                squawk.end(), [](char c) { return !std::isdigit(c) || c == '8' || c == '9'; }) == squawk.end();
        }
    }  // namespace Squawk
}  // namespace UKControllerPlugin

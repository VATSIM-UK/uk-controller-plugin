#include "pch/pch.h"
#include "metar/MetarParsingFunctions.h"

namespace UKControllerPlugin {
    namespace Metar {

        const std::string noQnh = "0000";

        /*
            Return the QNH string from a METAR if available.
        */
        std::string GetQnhString(std::string metar)
        {
            std::regex qnhPattern(" Q([0-9]{4})( |$)");
            std::smatch qnhMatch;

            if (!std::regex_search(metar, qnhMatch, qnhPattern)) {
                return noQnh;
            }

            return qnhMatch[1];
        }
    }  // namespace Metar
}  // namespace UKControllerPlugin

#include "pch/stdafx.h"
#include "datablock/DatablockFunctions.h"

namespace UKControllerPlugin {
    namespace Datablock {

        const std::regex flightLevelRegex("^(?:FL)?(\\d{2,3})$|^(\\d{4,5})$");

        int ConvertAltitudeToFlightLevel(int altitude)
        {
            return altitude / 100;
        }

        int ConvertFlightLevelToAltitude(int flightLevel)
        {
            return flightLevel * 100;
        }

        int NormaliseFlightLevelFromString(std::string level)
        {
            // Make it upper case
            std::transform(
                level.begin(),
                level.end(),
                level.begin(),
                [](unsigned char c) { return std::toupper(c); }
            );

            // Check for matches
            std::smatch matches;
            if (!std::regex_match(level, matches, flightLevelRegex)) {
                return -1;
            }

            int flightLevel = std::stoi(
                matches[1].str() != ""
                    ? matches[1].str()
                    : matches[2].str()
            );

            return flightLevel >= 1000
                       ? flightLevel / 100
                       : flightLevel;
        }
    }  // namespace Datablock
}  // namespace UKControllerPlugin

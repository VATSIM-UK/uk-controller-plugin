#pragma once

namespace UKControllerPlugin {
    namespace Datablock {

        int ConvertAltitudeToFlightLevel(int altitude);
        int ConvertFlightLevelToAltitude(int flightLevel);
        int NormaliseFlightLevelFromString(std::string level);
    }  // namespace Datablock
}  // namespace UKControllerPlugin

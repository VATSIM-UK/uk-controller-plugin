#include "pch/stdafx.h"
#include "datablock//DatablockFunctions.h"

namespace UKControllerPlugin {
    namespace Datablock {

        int ConvertAltitudeToFlightLevel(int altitude)
        {
            return altitude / 100;
        }

        int ConvertFlightLevelToAltitude(int flightLevel)
        {
            return flightLevel * 100;
        }
    }  // namespace Datablock
}  // namespace UKControllerPlugin

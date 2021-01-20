#include "pch/pch.h"
#include "datablock/DatablockFunctions.h"

using UKControllerPlugin::Datablock::ConvertAltitudeToFlightLevel;
using UKControllerPlugin::Datablock::ConvertFlightLevelToAltitude;

namespace UKControllerPluginTest {
    namespace Datablock {

        TEST(DatablockFunctions, AltitudesCanBeConvertedToFlightLevels)
        {
            EXPECT_EQ(350, ConvertAltitudeToFlightLevel(35000));
        }

        TEST(DatablockFunctions, FlightLevelsCanBeConvertedToAltitudes)
        {
            EXPECT_EQ(35000, ConvertFlightLevelToAltitude(350));
        }
    }  // namespace Datablock
}  // namespace UKControllerPluginTest

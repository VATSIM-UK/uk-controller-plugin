#pragma once

namespace UKControllerPlugin::Datablock {
    int ConvertAltitudeToFlightLevel(int altitude);
    int ConvertFlightLevelToAltitude(int flightLevel);
    int NormaliseFlightLevelFromString(std::string level);
    auto FrequencyStringFromDouble(double frequency) -> std::string;
} // namespace UKControllerPlugin::Datablock

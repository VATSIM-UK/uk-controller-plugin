#include "euroscope/RadarScreenCallbackFunction.h"

namespace UKControllerPlugin::Euroscope {

    RadarScreenCallbackFunction::RadarScreenCallbackFunction(
        int functionId,
        std::string description,
        std::function<void(int, EuroscopeRadarLoopbackInterface&, const std::string&, const POINT&, const RECT&)>
            function)
        : functionId(functionId), description(description), function(function)
    {
    }

    /*
        Assignment operator
    */
    void RadarScreenCallbackFunction::operator=(const RadarScreenCallbackFunction& copy)
    {
        this->functionId = copy.functionId;
        this->description = copy.description;
        this->function = copy.function;
    }

    /*
        Compares two callbacks, returns true if their id is the same.
    */
    bool RadarScreenCallbackFunction::operator==(const RadarScreenCallbackFunction& compare) const
    {
        return this->functionId == compare.functionId;
    }
} // namespace UKControllerPlugin::Euroscope

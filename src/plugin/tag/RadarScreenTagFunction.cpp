#include "RadarScreenTagFunction.h"
#include "tag/TagFunction.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Tag::TagFunction;
using is_transparent = int;

namespace UKControllerPlugin::Tag {

    RadarScreenTagFunction::RadarScreenTagFunction() = default;

    RadarScreenTagFunction::RadarScreenTagFunction(
        int functionId,
        std::string description,
        std::function<void(
            Euroscope::EuroscopeRadarLoopbackInterface&,
            EuroScopeCFlightPlanInterface&,
            EuroScopeCRadarTargetInterface&,
            std::string,
            const POINT& mousePos,
            const RECT& area)> function)
        : functionId(functionId), description(description), function(function)
    {
    }

    /*
        Copy constructor
    */
    RadarScreenTagFunction::RadarScreenTagFunction(const RadarScreenTagFunction& copy)
    {
        this->functionId = copy.functionId;
        this->description = copy.description;
        this->function = copy.function;
    }

    /*
        Assignment operator
    */
    void RadarScreenTagFunction::operator=(const RadarScreenTagFunction& copy)
    {
        this->functionId = copy.functionId;
        this->description = copy.description;
        this->function = copy.function;
    }

    /*
        Compares two callbacks, returns true if their id is the same.
    */
    bool RadarScreenTagFunction::operator==(const RadarScreenTagFunction& compare) const
    {
        return this->functionId == compare.functionId;
    }
} // namespace UKControllerPlugin::Tag

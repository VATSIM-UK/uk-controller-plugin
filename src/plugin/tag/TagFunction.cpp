#include "pch/pch.h"
#include "tag/TagFunction.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using is_transparent = int;

namespace UKControllerPlugin {
    namespace Tag {

        TagFunction::TagFunction()
        {
        }

        TagFunction::TagFunction(
            int functionId,
            std::string description,
            std::function<void(
                EuroScopeCFlightPlanInterface &,
                EuroScopeCRadarTargetInterface &,
                std::string,
                const POINT & mousePos
            )> function
        )
            : functionId(functionId), description(description), function(function)
        {

        }

        /*
            Assignment operator
        */
        void TagFunction::operator=(const TagFunction & copy)
        {
            this->functionId = copy.functionId;
            this->description = copy.description;
            this->function = copy.function;
        }

        /*
            Compares two callbacks, returns true if their id is the same.
        */
        bool TagFunction::operator==(const TagFunction & compare) const
        {
            return this->functionId == compare.functionId;
        }
    }  // namespace Tag
}  // namespace UKControllerPlugin

#include "pch/pch.h"
#include "euroscope/CallbackFunction.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        CallbackFunction::CallbackFunction(
            int functionId,
            std::string description,
            std::function<void(int, std::string, RECT)> function
        ) : functionId(functionId), description(description), function(function)
        {

        }

        /*
            Assignment operator
        */
        void CallbackFunction::operator=(const CallbackFunction & copy)
        {
            this->functionId = copy.functionId;
            this->description = copy.description;
            this->function = copy.function;
        }

        /*
            Compares two callbacks, returns true if their id is the same.
        */
        bool CallbackFunction::operator==(const CallbackFunction & compare) const
        {
            return this->functionId == compare.functionId;
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

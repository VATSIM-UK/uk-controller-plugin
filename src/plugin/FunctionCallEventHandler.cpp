#include "pch/stdafx.h"
#include "plugin/FunctionCallEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin {
    namespace Plugin {

        /*
            Call the given function.
        */
        void FunctionCallEventHandler::CallFunction(
            int functionId,
            std::string subject,
            EuroScopeCFlightPlanInterface &flightplan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) const {

            if (functionId < this->firstFixedId) {
                this->callbackFunctions.find(functionId)->function(subject);
            } else {
                this->tagFunctions.find(functionId)->function(flightplan, radarTarget);
            }
        }

        /*
            Returns the number of registered functions.
        */
        size_t FunctionCallEventHandler::CountCallbacks(void) const
        {
            return this->callbackFunctions.size();
        }

        /*
            Returns the number of tag functions.
        */
        size_t FunctionCallEventHandler::CountTagFunctions(void) const
        {
            return this->tagFunctions.size();
        }

        /*
            Returns the next function ID.
        */
        int FunctionCallEventHandler::ReserveNextDynamicFunctionId(void)
        {
            return this->nextDynamicFunctionId++;
        }

        /*
            Causes the collection to register all its TAG functions with the EuroScope plugin.
        */
        void FunctionCallEventHandler::RegisterTagFunctionsWithEuroscope(
            EuroscopePluginLoopbackInterface & plugin
        ) const {
            for (
                std::set<TagFunction>::const_iterator it = this->tagFunctions.cbegin();
                it != this->tagFunctions.cend();
                ++it
            ) {
                plugin.RegisterTagFunction(it->functionId, it->description);
            }
        }

        /*
            Registers a CallbackFunction with the handlers.
        */
        void FunctionCallEventHandler::RegisterFunctionCall(const CallbackFunction function)
        {
            if (this->callbackFunctions.insert(function).second == false) {
                throw std::invalid_argument("Function already registered for this id!");
            }
        }

        /*
            Registers a TagFunction with the handlers.
        */
        void FunctionCallEventHandler::RegisterFunctionCall(const UKControllerPlugin::Tag::TagFunction function)
        {
            if (this->tagFunctions.insert(function).second == false) {
                throw std::invalid_argument("Function already registered for this id!");
            }
        }
    }  // namespace Plugin
}  // namespace UKControllerPlugin

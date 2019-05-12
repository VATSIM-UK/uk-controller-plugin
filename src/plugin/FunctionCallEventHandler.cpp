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
            EuroScopeCRadarTargetInterface & radarTarget,
            const POINT & mousePos,
            const RECT & area
        ) const {

            if (functionId < this->firstFixedId) {
                auto function = this->callbackFunctions.find(functionId);
                if (function == this->callbackFunctions.cend()) {
                    LogWarning("Invalid dynamic callback function " + std::to_string(functionId));
                    return;
                }

                function->function(functionId, subject, area);
            } else {
                auto function = this->tagFunctions.find(functionId);
                if (function == this->tagFunctions.cend()) {
                    LogWarning("Invalid fixed callback function " + std::to_string(functionId));
                    return;
                }

                function->function(flightplan, radarTarget, subject, mousePos);
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
            Returns true if a callback function exists
        */
        bool FunctionCallEventHandler::HasCallbackFunction(int id) const
        {
            return this->callbackFunctions.find(id) != this->callbackFunctions.cend();
        }

        /*
            Returns true if a tag function exists
        */
        bool FunctionCallEventHandler::HasTagFunction(int id) const
        {
            return this->tagFunctions.find(id) != this->tagFunctions.cend();
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
